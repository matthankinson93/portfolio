const express = require('express');
const router = express.Router();
const { Pool } = require('pg');

const pool = new Pool({
  connectionString: process.env.DATABASE_URL
});

router.get('/latest/:deviceId', async (req, res) => {
  const { deviceId } = req.params;

  try {
    const latestTelemetryQuery = `
      SELECT DISTINCT ON (telemetry_key) telemetry_key, telemetry_value, telemetry_timestamp
      FROM device_telemetry
      WHERE device_id = $1
      ORDER BY telemetry_key, telemetry_timestamp DESC;
    `;
    
    const result = await pool.query(latestTelemetryQuery, [deviceId]);
    res.json(result.rows);
  } catch (error) {
    console.error(error);
    res.status(500).json({ success: false, message: 'Internal server error' });
  }
});

router.get('/range/:deviceId', async (req, res) => {
  const { deviceId } = req.params;
  const { startTS, endTS } = req.query;

  try {
    const rangeTelemetryQuery = `
      SELECT telemetry_key, telemetry_value, telemetry_timestamp
      FROM device_telemetry
      WHERE device_id = $1
      AND telemetry_timestamp BETWEEN $2 AND $3
      ORDER BY telemetry_key, telemetry_timestamp DESC;
    `;
    
    const result = await pool.query(rangeTelemetryQuery, [deviceId, startTS, endTS]);
    
    res.json(result.rows);
  } catch (error) {
    console.error(error);
    res.status(500).json({ success: false, message: 'Internal server error' });
  }
});

router.post('/saveTelemetry', verifyToken, async (req, res) => {
  const { key, value, ts } = req.body;
  const deviceToken = req.deviceToken;

  try {
    const deviceId = await getDeviceIdByToken(deviceToken);
    const telemetryTimestamp = ts;

    const insertTelemetryQuery = `
      INSERT INTO device_telemetry (device_id, telemetry_key, telemetry_value, telemetry_timestamp)
      VALUES ($1, $2, $3, $4)
      RETURNING *; // If you want to return the inserted row
    `;

    const insertResult = await pool.query(insertTelemetryQuery, [deviceId, key, value, telemetryTimestamp]);
    res.json({ success: true, message: 'Telemetry data saved successfully', data: insertResult.rows[0] });
  } catch (error) {
    console.error(error);
    res.status(500).json({ success: false, message: 'Internal server error' });
  }
});

module.exports = router;