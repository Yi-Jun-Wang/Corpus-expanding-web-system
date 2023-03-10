import React, { useContext, useState, useEffect, useRef } from 'react';
import 'antd/dist/antd.css';
import './App.css';
import { Navigate } from 'react-router-dom';
import { Table, Input, Form, Button, message } from 'antd';

const api_url = 'https://127.0.0.1:5000';
const EditableContext = React.createContext(null);
const InputText = (props) => {
  const test_sentence = '';
  const [sentence, setSent] = React.useState(test_sentence);
  const re = /([^_]+)/g;

  return (
    <div
      style={{
        display: 'flex',
        marginBottom: '10px',
      }}
    >
      <Input
        type="text"
        value={sentence}
        onPressEnter={() => {props.splitword(sentence, setSent)}}
        onChange={(e) => {
          setSent(e.target.value);
          if (e.target.value.includes('_')) {
            const new_datas = e.target.value.match(re);
            // console.log(new_datas);
            props.combine(new_datas);
          }
          else {
            props.combine(null);
          }
        }}
      ></Input>
      <Button 
        style={{
          marginLeft: '5px'
        }}
        type="primary"
        onClick={() => {props.splitword(sentence, setSent)}}
      >斷詞</Button>
      <Button 
        style={{
          marginLeft: '5px'
        }}
        type="primary"
        onClick={props.update}
      >更新詞庫</Button>
    </div>
  );
};

const EditableRow = ({ index, ...props }) => {
  const [form] = Form.useForm();
  return (
    <Form form={form} component={false}>
      <EditableContext.Provider value={form}>
        <tr {...props} />
      </EditableContext.Provider>
    </Form>
  );
};

const EditableCell = ({
  title,
  editable,
  children,
  dataIndex,
  record,
  handleSave,
  ...restProps
}) => {
  const [editing, setEditing] = useState(false);
  const inputRef = useRef(null);
  const form = useContext(EditableContext);
  useEffect(() => {
    if (editing) {
      inputRef.current.focus();
    }
  }, [editing]);

  const toggleEdit = () => {
    setEditing(!editing);
    form.setFieldsValue({
      [dataIndex]: record[dataIndex],
    });
  };

  const save = async () => {
    try {
      const values = await form.validateFields();
      toggleEdit();
      handleSave({ ...record, ...values });
    } catch (errInfo) {
      console.log('Save failed:', errInfo);
    }
  };

  let childNode = children;

  if (editable) {
    childNode = editing ? (
      <Form.Item
        style={{
          margin: 0,
        }}
        name={dataIndex}
        rules={[
          {
            required: true,
            messspell: `${title} is required.`,
          },
        ]}
      >
        <Input ref={inputRef} onPressEnter={save} onBlur={save} />
      </Form.Item>
    ) : (
      <div
        className="editable-cell-value-wrap"
        style={{
          paddingRight: 24,
        }}
        onClick={toggleEdit}
      >
        {children}
      </div>
    );
  }

  return <td {...restProps}>{childNode}</td>;
};

class EditableTable extends React.Component {
  constructor(props) {
    super(props);
    this.columns = [
      {
        title: '詞',
        dataIndex: 'pk',
        width: '10%',
      },
      {
        title: '四縣腔',
        dataIndex: 'spell1',
        editable: true,
      },
      {
        title: '海陸腔',
        dataIndex: 'spell2',
        editable: true,
      },
      {
        title: '大埔腔',
        dataIndex: 'spell3',
        editable: true,
      },
      {
        title: '饒平腔',
        dataIndex: 'spell4',
        editable: true,
      },
      {
        title: '詔安腔',
        dataIndex: 'spell5',
        editable: true,
      },
      {
        title: '南四縣腔',
        dataIndex: 'spell6',
        editable: true,
      },
      {
        title: '詞頻',
        dataIndex: 'freq',
        editable: true,
        width: '10%',
      },
      {
        title: '詞性',
        dataIndex: 'class',
        editable: true,
        width: '10%',
      },
    ];
    this.state = {
      dataSource: [],
      words: [],
    };
  }

  handleSave = (row) => {
    const newData = [...this.state.words];
    const index = newData.findIndex((item) => row.key === item.key);
    const item = newData[index];
    newData.splice(index, 1, { ...item, ...row });
    this.setState({
      words: newData,
    });
  };

  handleCombine = (new_words) => {
    if (!new_words) {
      this.setState({ words: [] });
      return;
    }
    const { dataSource } = this.state;
    // const priority = { '': 0, adj: 1, n: 2, pron: 3, adv: 4, v: 5 };
    let words = [];
    let new_word = {};
    let found = false;
    for (let i = 0; i < new_words.length; i++) {
      let result = dataSource.filter((w) => w.pk === new_words[i]);
      if (result.length !== 0) {
        let temp = {...result[0]};
        temp.key = `${i}`;
        words.push(temp);
      } else {
        found = false;
        new_word = { 
          key: `${i}`,
          pk: '',
          spell1: '',
          spell2: '',
          spell3: '',
          spell4: '',
          spell5: '',
          spell6: '',
          freq: '0',
          class: ''
        };
        let temp = new_words[i];
        let total_freq = 0, count = 0;
        for (let j = 0; j < dataSource.length; j++) {
          if (temp.search(dataSource[j].pk) === 0) {
            temp = temp.substring(dataSource[j].pk.length);
            new_word.pk += dataSource[j].pk;
            if (new_words[i].pk === new_word.pk) {
              console.log(new_word.pk)
              found = true;
            }
            new_word.spell1 += `${dataSource[j].spell1}`;
            new_word.spell2 += `${dataSource[j].spell2}`;
            new_word.spell3 += `${dataSource[j].spell3}`;
            new_word.spell4 += `${dataSource[j].spell4}`;
            new_word.spell5 += `${dataSource[j].spell5}`;
            new_word.spell6 += `${dataSource[j].spell6}`;
            // if (priority[new_word.class] < priority[dataSource[j].class]) {
            //   new_word.class = dataSource[j].class;
            // }
            new_word.class += `${dataSource[j].class},`;
            total_freq += parseInt(dataSource[j].freq);
            count++;
          } else if (found) {
            break;
          }
        }
        new_word.freq = Math.floor(total_freq/count);
        words.push(new_word);
      }
    }
    // console.log(words);
    this.setState({ words: [...words] });
  };

  handleSplit = (sentence, setSent) => {
    axios.post(`${api_url}/split`, {sent: sentence})
      .then((res) => {
        this.setState({ 
          words: [...res.data['words']],
          dataSource: [...res.data['words']]
        });
        setSent(res.data['sentence'])
        // console.log(res.data)
      })
      .catch((e) => {
        message.error(e.response.data['msg']);
      })
      .finally(() => {});
  }

  handleUpdate = () => {
    const {words} = this.state;
    const token = sessionStorage.getItem("token");
    const config = { 
      headers: {
        'Authorization': `Bearer ${token}`
      }
    }
    axios.post(`${api_url}/update`, {words: words}, config)
      .then((res) => {
        message.success(res.data['msg']);
      })
      .catch((e) => {
        const { setAuth } = this.props;
        message.error(e.response.data['msg']);
        if (e.response.data['msg'] === "Token has expired") {
          message.warning("登入過期，請重新登入");
          sessionStorage.removeItem("token");
          setAuth(false);
        }
      })
      .finally(() => {});
  }

  render() {
    const {words} = this.state;
    const {auth} = this.props;
    const components = {
      body: {
        row: EditableRow,
        cell: EditableCell,
      },
    };
    const columns = this.columns.map((col) => {
      if (!col.editable) {
        return col;
      }

      return {
        ...col,
        onCell: (record) => ({
          record,
          editable: col.editable,
          dataIndex: col.dataIndex,
          title: col.title,
          handleSave: this.handleSave,
        }),
      };
    });

    if (auth) {
      return (
        <div style={{
          width: '80vw',
          margin: '10px auto',
          minHeight: '600px',
          paddingBottom: '40px'
        }}>
          <InputText
          combine={(new_words) => {
            this.handleCombine(new_words);
          }}
          splitword={(sentence, setSent) => {
            this.handleSplit(sentence, setSent);
          }}
          update={this.handleUpdate}/>
          <Table
            components={components}
            rowClassName={'editable-row'}
            bordered
            dataSource={words}
            columns={columns}
            pagination={{
              position: ['bottomCenter']
            }}
          />
        </div>
      );        
    } 
    else {
      return (<Navigate to="/login" />);      
    }
  }
}

export default EditableTable;