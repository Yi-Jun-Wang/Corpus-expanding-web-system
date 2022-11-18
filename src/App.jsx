import { useState } from 'react'
import 'antd/dist/antd.css';
import './App.css'
import { Button, message, Upload, Progress, Input, Select } from 'antd';
import Segment_area from './Segment_area'
import Update_area from './Update_corp'

const { TextArea } = Input;
function App() {
  const [segment, setSegment] = useState('');

  return (
      <div className="App">
          <div id="Top-area" className="big-bg">
              <header className="page-header wrapper">
                  <h1><img className="logo" src="src/images/NYCU.png" alt="陽明交通大學" /></h1>
                  <nav>
                      <ul className="main-nav">
                          <li><a href="Login.html">Login</a></li>
                      </ul>
                  </nav>
              </header>

              <div className="wrapper">
                  <h2 className="page-title">客語詞庫擴充系統</h2>
              </div>
          </div>

          <Segment_area onChange={(value) => {setSegment(value)}}></Segment_area>

          <div className="Text_zone">
          <TextArea
            showCount
            value={segment}
            style={{resize: 'none', height: 120}}
            onChange={(e) => {setSegment(e.target.value)}}
          /></div>

          <Update_area></Update_area>

          <footer>
              <div className="wrapper">
                  <p><small>&copy; 2022 Jimmy</small></p>
              </div>
          </footer>

      </div>
        
  )
}

export default App
