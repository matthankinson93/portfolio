const express = require('express');
const router = express.Router();
const { Pool } = require('pg');
const crypto = require('crypto');

const pool = new Pool({
  connectionString: process.env.DATABASE_URL
});

function generateAuthToken() {
    return crypto.randomBytes(30).toString('hex');
}

router.get('/', async (req, res) => {
    try {
      const result = await pool.query('SELECT * FROM devices');
      res.json(result.rows);
    } catch (error) {
      console.error(error);
      res.status(500).json({ success: false, message: 'Internal server error' });
    }
  });
  
  router.get('/:id', async (req, res) => {
    try {
      const { id } = req.params;
      const result = await pool.query('SELECT * FROM devices WHERE device_id = $1', [id]);
      
      if (result.rows.length > 0) {
        const device = result.rows[0];
        res.json(device);
      } else {
        res.status(404).json({ success: false, message: 'Device not found' });
      }
    } catch (error) {
      console.error(error);
      res.status(500).json({ success: false, message: 'Internal server error' });
    }
  });
  
  router.post('/', async (req, res) => {
    try {
      const { name, owner_id } = req.body;
      const authToken = generateAuthToken();
  
      const insertQuery = `
        INSERT INTO devices (name, token, owner_id) 
        VALUES ($1, $2, $3) 
        RETURNING *;
      `;
      const values = [name, authToken, owner_id];
      
      const result = await pool.query(insertQuery, values);
  
      if (result.rows.length > 0) {
        const newDevice = result.rows[0];
        res.json({ success: true, message: 'New device created', device: newDevice });
      } else {
        res.status(400).json({ success: false, message: 'Device could not be created' });
      }
    } catch (error) {
      console.error(error);
      res.status(500).json({ success: false, message: 'Internal server error' });
    }
  });
  
  module.exports = router;