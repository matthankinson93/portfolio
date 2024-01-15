const express = require('express');
const router = express.Router();
const { Pool } = require('pg');

const pool = new Pool({
  connectionString: process.env.DATABASE_URL
});

async function verifyToken(req, res, next) {
  const deviceToken = req.headers['device-token'];

  if (deviceToken) {
    try {
      const query = 'SELECT device_id FROM devices WHERE token = $1';
      const result = await pool.query(query, [deviceToken]);
      if (result.rows.length > 0) {
        req.deviceId = result.rows[0].device_id;
        next();
      } else {
        res.status(401).json({ success: false, message: 'Invalid token' });
      }
    } catch (error) {
      console.error('Database error:', error);
      res.status(500).json({ success: false, message: 'Internal server error' });
    }
  } else {
    res.status(403).json({ success: false, message: 'No token provided' });
  }
}

async function getDeviceIdByToken(token) {
  const query = 'SELECT device_id FROM devices WHERE token = $1';
  const result = await pool.query(query, [token]);
  if (result.rows.length > 0) {
    return result.rows[0].device_id;
  } else {
    throw new Error('Invalid token');
  }
}

router.get('/', async (req, res) => {
    try {
      const result = await pool.query('SELECT * FROM users');
      res.json(result.rows);
    } catch (error) {
      console.error(error);
      res.status(500).json({ success: false, message: 'Internal server error' });
    }
  });

  router.get('/active', async (req, res) => {
    try {
      const result = await pool.query('SELECT * FROM users WHERE active = true');
      res.json(result.rows);
    } catch (error) {
      console.error(error);
      res.status(500).json({ success: false, message: 'Internal server error' });
    }
  });
  
  router.get('/:id', async (req, res) => {
    try {
      const { id } = req.params;
      const result = await pool.query('SELECT * FROM users WHERE user_id = $1', [id]);
      if (result.rows.length > 0) {
        res.json(result.rows[0]);
      } else {
        res.status(404).json({ success: false, message: 'User not found' });
      }
    } catch (error) {
      console.error(error);
      res.status(500).json({ success: false, message: 'Internal server error' });
    }
  });
  
  router.post('/', async (req, res) => {
    try {
      const { email } = req.body;
      const result = await pool.query('INSERT INTO users (email) VALUES ($1) RETURNING *', [email]);
      res.status(201).json(result.rows[0]);
    } catch (error) {
      console.error(error);
      res.status(500).json({ success: false, message: 'Internal server error' });
    }
  });

module.exports = router;