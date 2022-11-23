import 'antd/dist/antd.css';
import './App.css'
import { useState } from 'react'
import { Input } from 'antd';
import { Navigate } from 'react-router-dom'
import Segment_area from './Segment_area'

const { TextArea } = Input;
const Public_page = (props) => {
    const [segment, setSegment] = useState('');
    if (props.auth) {
      return <Navigate to="/private" />;
    }
    return (
        <div style={{
          minHeight: '500px'
        }}>

          <Segment_area onChange={(value) => {setSegment(value)}}></Segment_area>

          <div className="Text_zone">
          <TextArea
            showCount
            value={segment}
            style={{resize: 'none', height: 120}}
            onChange={(e) => {setSegment(e.target.value)}}
          /></div>

        </div>
    );
}

export default Public_page;