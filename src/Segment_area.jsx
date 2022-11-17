class Segment_area extends React.Component {
    
    constructor(props) {
        super(props);

        this.state = {
            fileList: [],
            uploading: false,
            progress: 0,
            upload_stat: 'active',
            segment: '',
            accent: '0',
            spell: '0',
            format: '0'
        };
    }

    handle_segment = (mode=true) => {
        let formData = new FormData();
        fileList2.forEach((file) => {
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
            setProgress2(percentCompleted);
            //console.log(percentCompleted);
          }
        };
        setStat2('active');
        axios.post(
          `http://10.10.8.42:5000/segment/${accent}/${spell_type}/${out_format}`,
          formData,
          config)
        .then((response) => {
          setFileList2([]);
          console.log('上傳成功');
          message.success('上傳成功');
          setStat2('');
          setSegment(response.data['content']);
          if (mode) {
            window.location.href = 'http://10.10.8.42:5000/segment';
          }
        })
        .catch((e) => {
          console.log(e);
          message.error('upload failed.');
          setStat2('exception');
        })
        .finally(() => {
          setUploading(false);
        });
      }
    
    handle_segment_no_file = () => {
        handle_segment(false);
    }

    render() {
        return (
            <div className="ant-dragger">
                <h2 className="hint">斷詞結果回傳</h2>
                <Upload.Dragger
                onRemove={(file) => {
                    const index = fileList2.indexOf(file);
                    const newFileList = fileList2.slice();
                    newFileList.splice(index, 1);
                    setFileList2(newFileList);
                }}
                onChange={(info) => {
                    setFileList2([...info.fileList]);
                }}
                beforeUpload={(file) => {
                    setFileList2([...fileList2, file]);
                    setStat2('');
                    setProgress2(0);
                    return false;
                }}
                fileList={fileList2}
                maxCount={1}
                >
                <p className="ant-upload-drag-icon">
                    <InboxOutlined />
                </p>
                <p className="ant-upload-text">
                    點擊或拖放檔案上傳
                </p>
                </Upload.Dragger>

                <Progress percent={progress2} status={upload_stat2}></Progress>
                
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
                    onChange={(value) => {setAccent(value)}}
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
                    onChange={(value) => {setSpell(value)}}
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
                    onChange={(value) => {setFormat(value)}}
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
                    onClick={handle_segment_no_file}
                    disabled={fileList2.length === 0}
                    loading={uploading}
                    >{uploading ? '上傳中' : '上傳'}
                    </Button>
                    <Button
                    type="primary"
                    onClick={handle_segment}
                    disabled={fileList2.length === 0}
                    loading={uploading}
                    >{uploading ? '上傳中' : '上傳並下載'}
                    </Button>
                </div>
                </div>
            </div>
        );
    }
}