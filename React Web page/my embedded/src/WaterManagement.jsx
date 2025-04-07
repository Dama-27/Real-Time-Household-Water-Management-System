import React, { useState, useEffect } from "react";
import { LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, Legend } from "recharts";
import logo from "./assets/logo.png";
import waterImage from "./assets/water.jpg";
import solenoidImage from "./assets/valve.jpg";
import pumpImage from "./assets/water pump.png";

// Firebase connection
import { initializeApp } from "firebase/app";
import { getDatabase, ref, onValue, set } from "firebase/database";

// Firebase Configuration
const firebaseConfig = {
  apiKey: "AIzaSyD-DoytcfFd_NdvcpyppC1cb3biFM1-kVE",
  authDomain: "smart-house-water-management.firebaseapp.com",
  databaseURL: "https://smart-house-water-management-default-rtdb.firebaseio.com",
  projectId: "smart-house-water-management",
  storageBucket: "smart-house-water-management.appspot.com",
  messagingSenderId: "373754444462",
  appId: "1:373754444462:web:124a34cf2711d06c790b3f",
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);
const realtimeDB = getDatabase(app); // Realtime Database

const WaterManagement = () => {
  const [temperature, setTemperature] = useState(25);
  const [tds, setTds] = useState(300);
  const [waterQuality, setWaterQuality] = useState("Good");
  const [waterLevel, setWaterLevel] = useState(75);
  const [motorOn, setMotorOn] = useState(false);
  const [inflowRate, setInflowRate] = useState(10);
  const [outflowRate, setOutflowRate] = useState(8);
  const [temperatureData, setTemperatureData] = useState([]);
  const [waterUsageData, setWaterUsageData] = useState([]);
  const [inflowData, setInflowData] = useState([]);
  const [outflowData, setOutflowData] = useState([]);
  const [inSolenoidOn, setInSolenoidOn] = useState(false);
  const [outSolenoidOn, setOutSolenoidOn] = useState(false);
  const [toggleMessage, setToggleMessage] = useState("");

  // Fetch data from Firebase Realtime Database
  useEffect(() => {
    const temperatureRef = ref(realtimeDB, "Temperature");
    const tdsRef = ref(realtimeDB, "TDS");
    const waterQualityRef = ref(realtimeDB, "WaterQuality");
    const waterLevelRef = ref(realtimeDB, "WaterLevel");
    const inflowRateRef = ref(realtimeDB, "Inflow");
    const outflowRateRef = ref(realtimeDB, "Outflow");

    const unsubscribeTemperature = onValue(temperatureRef, (snapshot) => {
      setTemperature(snapshot.val());
    });

    const unsubscribeTds = onValue(tdsRef, (snapshot) => {
      setTds(snapshot.val());
    });

    const unsubscribeWaterQuality = onValue(waterQualityRef, (snapshot) => {
      setWaterQuality(snapshot.val());
    });

    const unsubscribeWaterLevel = onValue(waterLevelRef, (snapshot) => {
      setWaterLevel(snapshot.val());
    });

    const unsubscribeInflowRate = onValue(inflowRateRef, (snapshot) => {
      setInflowRate(snapshot.val());
    });

    const unsubscribeOutflowRate = onValue(outflowRateRef, (snapshot) => {
      setOutflowRate(snapshot.val());
    });

    return () => {
      unsubscribeTemperature();
      unsubscribeTds();
      unsubscribeWaterQuality();
      unsubscribeWaterLevel();
      unsubscribeInflowRate();
      unsubscribeOutflowRate();
    };
  }, []);

  // Update motorOn state in Firebase
  const handleMotorSwitch = () => {
    const newMotorState = !motorOn;
    setMotorOn(newMotorState);
    set(ref(realtimeDB, "MotorSwich"), newMotorState);
  };

  // Update inSolenoidOn state in Firebase
  const handleInSolenoidSwitch = () => {
    const newInSolenoidState = !inSolenoidOn;
    setInSolenoidOn(newInSolenoidState);
    set(ref(realtimeDB, "InValve"), newInSolenoidState);
  };

  // Update outSolenoidOn state in Firebase
  const handleOutSolenoidSwitch = () => {
    const newOutSolenoidState = !outSolenoidOn;
    setOutSolenoidOn(newOutSolenoidState);
    set(ref(realtimeDB, "OutValve"), newOutSolenoidState);
  };

  // Toggle feature control
  const handleToggle = () => {
    const newMessage = toggleMessage === "Water Connection" ? "Motor Line" : "Water Connection";
    setToggleMessage(newMessage);
    set(ref(realtimeDB, "toggleState"), newMessage);
  };

  // Log data every 5 seconds for charts
  useEffect(() => {
    const interval = setInterval(() => {
      const time = new Date().toLocaleTimeString();
      setTemperatureData((prev) => [...prev, { time, value: temperature }]);
      setWaterUsageData((prev) => [...prev, { time, value: waterLevel }]);
      setInflowData((prev) => [...prev, { time, value: inflowRate }]);
      setOutflowData((prev) => [...prev, { time, value: outflowRate }]);
    }, 5000);
    return () => clearInterval(interval);
  }, [temperature, waterLevel, inflowRate, outflowRate]);

  return (
    <div className="main-container">
      {/* Header */}
      <header className="header">
        <img src={logo} alt="System Logo" className="logo" />
        <h1>Smart Water Management System</h1>
      </header>

      <div className="container">
        {/* Water Parameters */}
        <div className="card">
          <h2>Water Parameters</h2>
          <img src={waterImage} alt="Water" className="water-parameters" />
          <p>Temperature: {temperature}°C</p>
          <p>TDS: {tds} ppm</p>
          <p>Water Quality: {waterQuality}</p>
          <p>Water Level: {waterLevel}%</p>
          <p>Inflow Rate: {inflowRate} L/min</p>
          <p>Outflow Rate: {outflowRate} L/min</p>
        </div>

        {/* Motor Control */}
        <div className="card motor-control">
          <div className="control-button">
            <h2>Motor Control</h2>
            <p>Status: {motorOn ? "ON" : "OFF"}</p>
            <button className="button" onClick={handleMotorSwitch}>
              {motorOn ? "Turn Off" : "Turn On"}
            </button>

            {/* Toggle Button */}
            <h2>Feature Control</h2>
            <p>Feature Toggle:</p>
            <div className="toggle-switch" onClick={handleToggle}>
              <div className={`toggle-thumb ${toggleMessage === "Water Connection" ? "on" : ""}`} />
            </div>
            <span className="toggle-message">{toggleMessage}</span>
          </div>
          <img src={pumpImage} alt="Motor" className="card-image motor-image" />
        </div>

        {/* Solenoid Valve Controls */}
        <div className="card solenoid-control center">
          <h2>Solenoid Valve Control</h2>
          <p>In Solenoid Valve: {inSolenoidOn ? "ON" : "OFF"}</p>
          <button className="button" onClick={handleInSolenoidSwitch}>
            {inSolenoidOn ? "Turn Off In Solenoid" : "Turn On In Solenoid"}
          </button>
          <p>Out Solenoid Valve: {outSolenoidOn ? "ON" : "OFF"}</p>
          <button className="button" onClick={handleOutSolenoidSwitch}>
            {outSolenoidOn ? "Turn Off Out Solenoid" : "Turn On Out Solenoid"}
          </button>
          <div></div>
          <img src={solenoidImage} alt="Solenoid Valve" className="card-image solenoid-image" />
        </div>

        {/* Charts */}
        <div className="card">
          <h2>Temperature</h2>
          <LineChart width={600} height={300} data={temperatureData}>
            <XAxis dataKey="time" tick={{ fill: "#000000", fontSize: 14 }} />
            <YAxis tick={{ fill: "#000000", fontSize: 14 }} />
            <CartesianGrid stroke="#000000" strokeDasharray="3 3" />
            <Tooltip />
            <Legend />
            <Line type="monotone" dataKey="value" stroke="#8884d8" />
          </LineChart>
        </div>

        <div className="card">
          <h2>Water Usage</h2>
          <LineChart width={600} height={300} data={waterUsageData}>
            <XAxis dataKey="time" tick={{ fill: "#000000", fontSize: 14 }} />
            <YAxis tick={{ fill: "#000000", fontSize: 14 }} />
            <CartesianGrid stroke="#000000" strokeDasharray="3 3" />
            <Tooltip />
            <Legend />
            <Line type="monotone" dataKey="value" stroke="#82ca9d" />
          </LineChart>
        </div>

        <div className="card">
          <h2>Inflow Rate</h2>
          <LineChart width={600} height={300} data={inflowData}>
            <XAxis dataKey="time" tick={{ fill: "#000000", fontSize: 14 }} />
            <YAxis tick={{ fill: "#000000", fontSize: 14 }} />
            <CartesianGrid stroke="#000000" strokeDasharray="3 3" />
            <Tooltip />
            <Legend />
            <Line type="monotone" dataKey="value" stroke="#ff7300" />
          </LineChart>
        </div>

        <div className="card">
          <h2>Outflow</h2>
          <LineChart width={600} height={300} data={outflowData}>
            <XAxis dataKey="time" tick={{ fill: "#000000", fontSize: 14 }} />
            <YAxis tick={{ fill: "#000000", fontSize: 14 }} />
            <CartesianGrid stroke="#000000" strokeDasharray="3 3" />
            <Tooltip />
            <Legend />
            <Line type="monotone" dataKey="value" stroke="#d84f4f" />
          </LineChart>
        </div>
      </div>
    </div>
  );
};

export default WaterManagement;