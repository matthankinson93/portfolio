/* MagicMirror²
 * Server
 *
 * By Michael Teeuw https://michaelteeuw.nl
 * MIT Licensed.
 */
const fs = require("fs");
const http = require("http");
const https = require("https");
const path = require("path");
const express = require("express");
const ipfilter = require("express-ipfilter").IpFilter;
const helmet = require("helmet");
const socketio = require("socket.io");
const fetch = require('node-fetch');
const apiKeys = JSON.parse(fs.readFileSync('apiKeys.json'));
const Log = require("logger");
const Utils = require("./utils");
const { cors, getConfig, getHtml, getVersion } = require("./server_functions");
const openAIkey = apiKeys.openAIkey;
const blynkBabyKey = apiKeys.blynkBabyKey;
const blynkKegKey = apiKeys.blynkKegKey;

/**
 * Server
 * @param {object} config The MM config
 * @class
 */
function Server(config) {
	const app = express();
	const port = process.env.MM_PORT || config.port;
	const serverSockets = new Set();
	let server = null;

	/**
	 * Opens the server for incoming connections
	 * @returns {Promise} A promise that is resolved when the server listens to connections
	 */
	this.open = function () {
		return new Promise((resolve) => {
			if (config.useHttps) {
				const options = {
					key: fs.readFileSync(config.httpsPrivateKey),
					cert: fs.readFileSync(config.httpsCertificate)
				};
				server = https.Server(options, app);
			} else {
				server = http.Server(app);
			}
			const io = socketio(server, {
				cors: {
					origin: /.*$/,
					credentials: true
				},
				allowEIO3: true
			});
			app.use(express.json());
			server.on("connection", (socket) => {
				serverSockets.add(socket);
				socket.on("close", () => {
					serverSockets.delete(socket);
				});
			});
			
			Log.log(`Starting server on port ${port} ... `);
			server.listen(port, config.address || "localhost");

			if (config.ipWhitelist instanceof Array && config.ipWhitelist.length === 0) {
				Log.warn(Utils.colors.warn("You're using a full whitelist configuration to allow for all IPs"));
			}

			app.use(function (req, res, next) {
				ipfilter(config.ipWhitelist, { mode: config.ipWhitelist.length === 0 ? "deny" : "allow", log: false })(req, res, function (err) {
					if (err === undefined) {
						res.header("Access-Control-Allow-Origin", "*");
						return next();
					}
					Log.log(err.message);
					res.status(403).send("This device is not allowed to access your mirror. <br> Please check your config.js or config.js.sample to change this.");
				});
			});
			
			app.use(helmet(config.httpHeaders));
			app.use("/js", express.static(__dirname));

			// TODO add tests directory only when running tests?
			const directories = ["/config", "/css", "/fonts", "/modules", "/vendor", "/translations", "/tests/configs", "/tests/mocks"];
			for (const directory of directories) {
				app.use(directory, express.static(path.resolve(global.root_path + directory)));
			}

			app.get("/cors", async (req, res) => await cors(req, res));

			app.get("/version", (req, res) => getVersion(req, res));

			app.get("/config", (req, res) => getConfig(req, res));

			app.get("/", (req, res) => getHtml(req, res));

			app.get("/blynkAPIBaby", async (req, res) => {
   	 			const BLYNK_AUTH_TOKEN = blynkBabyKey;
    			const blynkBaseUrl = "https://blynk.cloud/external/api/";
    			try {
        			const response = await fetch(`${blynkBaseUrl}get?token=${BLYNK_AUTH_TOKEN}&v0&v1&v2&v3`);
        			const data = await response.json();
					res.json({ reply: data });
    			} catch (error) {
        			console.error("Error fetching from Blynk:", error);
        			res.status(500).json({ error: "Failed to fetch from Blynk" });
    			}
			});

			app.get("/blynkAPIKegs", async (req, res) => {
				const BLYNK_AUTH_TOKEN = blynkKegKey;
				const blynkBaseUrl = "https://blynk.cloud/external/api/";
				try {
					const response = await fetch(`${blynkBaseUrl}get?token=${BLYNK_AUTH_TOKEN}&v0&v1&v2`);
					const data = await response.json();
					res.json({ reply: data });
				} catch (error) {
					console.error("Error fetching from Blynk:", error);
					res.status(500).json({ error: "Failed to fetch from Blynk" });
				}
			});

			app.post("/gptPrompt", async (req, res) => {
				const endpoint = "https://api.openai.com/v1/chat/completions";
				const apiKey = openAIkey;
			
				const headers = {
					"Authorization": `Bearer ${apiKey}`,
					"Content-Type": "application/json",
					"Accept": "application/json",
				};
			
				const body = JSON.stringify({
					model: "gpt-3.5-turbo",
					messages: [
						{
							role: "user",
							content: req.body.promptText
						}
					],
					temperature: 0.7,
					max_tokens: 600,
				});
			
				try {
					const response = await fetch(endpoint, {
						method: "POST",
						headers: headers,
						body: body,
					});
			
					const data = await response.json();
					if (data.choices[0].message.content) {
						res.json({ reply: data.choices[0].message.content });
					} else {
						throw new Error("Invalid response from GPT-3.5 Turbo");
					}
				} catch (error) {
					console.error("Error fetching from OpenAI:", error);
					res.status(500).json({ error: "Failed to fetch from OpenAI" });
				}
			});

			server.on("listening", () => {
				resolve({
					app,
					io
				});
			});
		});
	};

	/**
	 * Closes the server and destroys all lingering connections to it.
	 * @returns {Promise} A promise that resolves when server has successfully shut down
	 */
	this.close = function () {
		return new Promise((resolve) => {
			for (const socket of serverSockets.values()) {
				socket.destroy();
			}
			server.close(resolve);
		});
	};
}

module.exports = Server;
