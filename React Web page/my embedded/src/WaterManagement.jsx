import React, { useState, useEffect } from "react";
import { LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, Legend } from "recharts";
import logo from "./assets/logo.png";
import waterImage from "./assets/water.jpg";
import solenoidImage from "./assets/valve.jpg";
import pumpImage from "./assets/water pump.png";

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

  const handleMotorSwitch = () => {
    setMotorOn(!motorOn);
  };

  const handleInSolenoidSwitch = () => {
    setInSolenoidOn(!inSolenoidOn);
  };

  const handleOutSolenoidSwitch = () => {
    setOutSolenoidOn(!outSolenoidOn);
  };

  return (
    <div className="main-container">
      {/* Header*/}
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
          <h2>Motor Control</h2>
          <p>Status: {motorOn ? "ON" : "OFF"}</p>
          <button className="button" onClick={handleMotorSwitch}>
            {motorOn ? "Turn Off" : "Turn On"}
          </button>
          <div></div>
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
