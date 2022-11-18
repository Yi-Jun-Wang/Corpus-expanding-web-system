import { useState } from 'react'
import 'antd/dist/antd.css';
import './App.css'
import { InboxOutlined } from '@ant-design/icons';
import { Button, message, Upload, Progress, Input, Select } from 'antd';
import Segment_area from './Segment_area'

const { TextArea } = Input;
function App() {
  const [fileList, setFileList] = useState([]);
  const [uploading, setUploading] = useState(false);
  const [progress, setProgress] = useState(0);
  const [upload_stat, setStat] = useState('active');
  const [segment, setSegment] = useState('');
  
  const handleUpload = () => {
    let formData = new FormData();
    fileList.forEach((file) => {
      //console.log(file.name);
      formData.append('name', file.name);
      formData.append('file', file.originFileObj);
    });
    setUploading(true);
    const config = {
      headers: {'Content-Type': 'multipart/form-data'},
      onUploadProgress: function (progressEvent) {
        var percentCompleted = Math.round(
          (progressEvent.loaded / progressEvent.total) * 100
        );
        setProgress(percentCompleted);
        //console.log(percentCompleted);
      }
    };
    setStat('active');
    axios.post('http://10.10.8.42:5000/update_corpus', formData, config)
    .then((response) => {
        setFileList([]);
        if (response.data['response'] === '更新成功') {
          console.log('更新成功');
          message.success('更新成功');
          setStat('');
        } else {
          console.log(response.data['response']);
          message.error(response.data['response']);
          setStat('exception');
        }
      })
      .catch((e) => {
        console.log(e);
        message.error('upload failed.');
        setStat('exception');
      })
      .finally(() => {
        setUploading(false);
      });
  };

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

          <div className="ant-dragger">
            <h2 className="hint">詞庫更新</h2>
            <Upload.Dragger
              onRemove={(file) => {
                const index = fileList.indexOf(file);
                const newFileList = fileList.slice();
                newFileList.splice(index, 1);
                setFileList(newFileList);
              }}
              onChange={(info) => {
                setFileList([...info.fileList]);
              }}
              beforeUpload={(file) => {
                setFileList([...fileList, file]);
                setStat('');
                setProgress(0);
                return false;
              }}
              fileList={fileList}
              maxCount={1}
            >
              <p className="ant-upload-drag-icon">
                <InboxOutlined />
              </p>
              <p className="ant-upload-text">
                點擊或拖放檔案上傳
              </p>
            </Upload.Dragger>
            
            <Progress percent={progress} status={upload_stat}></Progress>

            <div className="load_button">
              <Button
                type="primary"
                onClick={handleUpload}
                disabled={fileList.length === 0}
                loading={uploading}
              >{uploading ? '上傳中' : '上傳'}
              </Button>
              <a href="http://10.10.8.42:5000/download" target="_blank">
                <Button
                  type="primary">下載詞庫
                </Button>
              </a>
            </div>
          </div>

          <footer>
              <div className="wrapper">
                  <p><small>&copy; 2022 Jimmy</small></p>
              </div>
          </footer>

      </div>
        
  )
}

export default App
