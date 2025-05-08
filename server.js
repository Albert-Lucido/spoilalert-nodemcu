const express = require('express');
const mongoose = require('mongoose');
const bodyParser = require('body-parser');
const cors = require('cors');

// Initialize app
const app = express();
const PORT = 3000;

// Middleware
app.use(cors());
app.use(bodyParser.json());

// MongoDB connection
mongoose.connect('mongodb+srv://AJ:veggie@veggiedata.gdafs7i.mongodb.net/?retryWrites=true&w=majority')
  .then(() => console.log('MongoDB connected'))
  .catch(err => {
    console.error('MongoDB connection error:', err);
    process.exit(1); // Exit if cannot connect to database
  });

// Mongoose schema
const SensorDataSchema = new mongoose.Schema({
  temperature: Number,
  humidity: Number,
  co2: Number,
  ethylene: Number,
  vegetable: String,
  timestamp: {
    type: Date,
    default: Date.now,
  },
});

// Mongoose model
const SensorData = mongoose.model('SensorData', SensorDataSchema);

// Route to handle incoming sensor data
app.post('/api/data', async (req, res) => {
  const { temperature, humidity, co2, ethylene, vegetable } = req.body;

  if (
    temperature === undefined || humidity === undefined ||
    co2 === undefined || ethylene === undefined || !vegetable
  ) {
    return res.status(400).json({ message: 'Missing required fields' });
  }

  try {
    const newData = new SensorData({ temperature, humidity, co2, ethylene, vegetable });
    await newData.save();
    console.log('📦 Data saved:', newData);
    res.status(200).json({ message: 'Data saved successfully' });
  } catch (err) {
    console.error('❌ Error saving data:', err);
    res.status(500).json({ message: 'Server error' });
  }
});

// Start server
app.listen(PORT, () => {
  console.log(`🚀 Server running on port ${PORT}`);
});
