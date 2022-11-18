import React from 'react';
import 'antd/dist/antd.css';
import { InboxOutlined } from '@ant-design/icons';
import { Button, message, Upload, Progress } from 'antd';

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
        
        const config = {
          headers: {'Content-Type': 'multipart/form-data'},
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
            
        axios.post('http://10.10.8.42:5000/update_corpus', formData, config)
        .then((response) => {
            this.setState({fileList: []});
            if (response.data['response'] === '更新成功') {
              console.log('更新成功');
              message.success('更新成功');
              this.setState({upload_stat: ''});
            } else {
              console.log(response.data['response']);
              message.error(response.data['response']);
              this.setState({upload_stat: 'exception'});
            }
          })
          .catch((e) => {
            console.log(e);
            message.error('upload failed.');
            this.setState({upload_stat: 'exception'});
          })
          .finally(() => {
            this.setState({uploading: false});
          });
      };

    render() {
        return (
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
                    type="primary"
                    onClick={this.handleUpload}
                    disabled={this.state.fileList.length === 0}
                    loading={this.state.uploading}
                >{this.state.uploading ? '上傳中' : '上傳'}
                </Button>
                <a href="http://10.10.8.42:5000/download" target="_blank">
                    <Button
                    type="primary">下載詞庫
                    </Button>
                </a>
                </div>
            </div>
        );
    }
}
export default Update_area;