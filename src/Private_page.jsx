import 'antd/dist/antd.css';
import './App.css'
import { useState, useEffect } from 'react'
import { Input, message } from 'antd';
import { Navigate } from 'react-router-dom';
import Segment_area from './Segment_area'
import Update_area from './Update_corp'

const { TextArea } = Input;
const Private_page = (props) => {
  const [segment, setSegment] = useState('');
  if (props.auth) {
    return (
        <div style={{
            height: '90vh',
            minHeight: '700px'
        }}>
            <Segment_area onChange={(value) => {setSegment(value)}}></Segment_area>

            <div className="Text_zone">
            <TextArea
                showCount
                value={segment}
                style={{resize: 'none', height: 120}}
                onChange={(e) => {setSegment(e.target.value)}}
            /></div>

            <Update_area></Update_area>

        </div>       
    );
  }
  else {
    useEffect( () => { 
      message.warning("登入後才可進入該頁面！");
    }, []);
    return <Navigate to="/login" />;
  }
}

export default Private_page;