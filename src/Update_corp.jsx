import React from 'react';
import 'antd/dist/antd.css';
import { InboxOutlined } from '@ant-design/icons';
import { Button, message, Upload, Progress } from 'antd';

// const api_url = "https://10.10.8.42:5000";
// const api_url = 'https://140.113.170.220:5000';
const api_url = 'https://127.0.0.1:5000';
class Update_area extends React.Component {

    constructor(props) {
        super(props);

        this.state = {
            fileList: [],
            uploading: false,
            progress: 0,
            upload_stat: 'active',
        };
    }

    handleUpload = () => {
        let formData = new FormData();
        this.state.fileList.forEach((file) => {
          //console.log(file.name);
          formData.append('name', file.name);
          formData.append('file', file.originFileObj);
        });

        const token = sessionStorage.getItem("token");
        const config = { 
          headers: {
            'Content-Type': 'multipart/form-data',
            'Authorization': `Bearer ${token}`},
          onUploadProgress: (progressEvent) => {
            var percentCompleted = Math.round(
              (progressEvent.loaded / progressEvent.total) * 100
            );
            this.setState({progress: percentCompleted});
          }
        };
        
        this.setState({
            uploading: true,
            upload_stat: 'active'});
            
        axios.post(`${api_url}/update_corpus`, formData, config)
        .then((res) => {
            this.setState({fileList: []});
            message.success('更新成功');
            this.setState({upload_stat: ''});
          })
          .catch((e) => {
            console.log(e);
            message.error(e.response.data['msg']);
            this.setState({upload_stat: 'exception'});
          })
          .finally(() => {
            this.setState({uploading: false});
          });
      };

    render() {
        return (
            <div style={{
                minHeight: '45vh'
            }}>
                <div className="ant-dragger">
                    <h2 className="hint">詞庫更新</h2>
                    <Upload.Dragger
                    onRemove={(file) => {
                        const index = this.state.fileList.indexOf(file);
                        const newFileList = this.state.fileList.slice();
                        newFileList.splice(index, 1);
                        this.setState({fileList:newFileList});
                    }}
                    onChange={(info) => {
                        this.setState({fileList:[...info.fileList]});
                    }}
                    beforeUpload={(file) => {
                        this.setState({
                            fileList:[...this.state.fileList, file],
                            upload_stat: '',
                            progress: 0,
                        });
                        return false;
                    }}
                    fileList={this.state.fileList}
                    maxCount={1}
                    >
                    <p className="ant-upload-drag-icon">
                        <InboxOutlined />
                    </p>
                    <p className="ant-upload-text">
                        點擊或拖放檔案上傳
                    </p>
                    </Upload.Dragger>
                    
                    <Progress percent={this.state.progress} status={this.state.upload_stat}></Progress>

                    <div className="load_button">
                    <Button
                        style={{
                            marginTop:'15px',
                        }}
                        type="primary"
                        onClick={this.handleUpload}
                        disabled={this.state.fileList.length === 0}
                        loading={this.state.uploading}
                    >{this.state.uploading ? '上傳中' : '上傳'}
                    </Button>
                    <a href={`${api_url}/download`} target="_blank">
                        <Button
                            style={{
                                marginTop:'15px',
                            }}
                            type="primary">下載詞庫
                        </Button>
                    </a>
                    </div>
                </div>
            </div>
        );
    }
}
export default Update_area;