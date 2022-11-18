import React from 'react';
import 'antd/dist/antd.css';
import { InboxOutlined } from '@ant-design/icons';
import { Button, message, Upload, Progress, Input, Select } from 'antd';
class Segment_area extends React.Component {
    
    constructor(props) {
        super(props);

        this.state = {
            fileList: [],
            uploading: false,
            progress: 0,
            upload_stat: 'active',
            accent: '1',
            spell: '0',
            format: '0',
        };
    }

    handle_segment = (mode=true) => {
        const { onChange } = this.props;
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
            //console.log(percentCompleted);
          }
        };
        this.setState({
            uploading: true,
            upload_stat: 'active'});
        const { accent, spell, format } = this.state;
        axios.post(
          `http://10.10.8.42:5000/segment/${accent}/${spell}/${format}`,
          formData,
          config)
        .then((response) => {
            console.log('上傳成功');
            message.success('上傳成功');
            this.setState({
                 fileList: [],
                 upload_stat: ''});
                
            if (onChange != undefined){
                onChange(response.data['content']);
            }
            if (mode) {
                window.location.href = 'http://10.10.8.42:5000/segment';
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
      }
    
    handle_segment_no_file = () => {
        this.handle_segment(false);
    }

    render() {
        return (
            <div className="ant-dragger">
                <h2 className="hint">斷詞結果回傳</h2>
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
                
                <div style={{
                display: 'flex',
                //position: 'relative',
                }}>
                <div style={{
                    marginTop: 15,
                    //position: 'absolute',
                    //width: '15vw',
                    minWidth: '375px',
                }}><Select
                    defaultValue="1"
                    style={{
                    width: 120,
                    }}
                    onChange={(value) => {this.setState({accent: value});}}
                    options={[
                    {
                        value: '0',
                        label: '大埔腔',
                    },
                    {
                        value: '1',
                        label: '四縣腔',
                    },
                    {
                        value: '2',
                        label: '南四縣腔',
                    },
                    {
                        value: '3',
                        label: '海陸腔',
                    },
                    {
                        value: '4',
                        label: '紹安腔',
                    },
                    {
                        value: '5',
                        label: '饒平腔',
                    },
                    ]}
                />
                <Select
                    defaultValue="0"
                    style={{
                    width: 120,
                    }}
                    onChange={(value) => {this.setState({spell: value});}}
                    options={[
                    {
                        value: '0',
                        label: '羅馬音',
                    },
                    {
                        value: '1',
                        label: '注音',
                    },
                    {
                        value: '2',
                        label: '羅馬音 + 注音',
                    },
                    ]}
                />
                <Select
                    defaultValue="0"
                    style={{
                    width: 120,
                    }}
                    onChange={(value) => {this.setState({format:value});}}
                    options={[
                    {
                        value: '0',
                        label: 'CSV',
                    },
                    {
                        value: '1',
                        label: 'TXT',
                    },
                    ]}
                /></div>
                <div style={{
                    display: 'flex',
                    justifyContent: 'space-between',
                    width: '11vw',
                }}>
                    <Button
                    type="primary"
                    onClick={this.handle_segment_no_file}
                    disabled={this.state.fileList.length === 0}
                    loading={this.state.uploading}
                    >{this.state.uploading ? '上傳中' : '上傳'}
                    </Button>
                    <Button
                    type="primary"
                    onClick={this.handle_segment}
                    disabled={this.state.fileList.length === 0}
                    loading={this.state.uploading}
                    >{this.state.uploading ? '上傳中' : '上傳並下載'}
                    </Button>
                </div>
                </div>
            </div>
        );
    }
}

export default Segment_area;