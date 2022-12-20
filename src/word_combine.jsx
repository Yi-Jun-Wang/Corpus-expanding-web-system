import React, { useContext, useState, useEffect, useRef } from 'react';
import ReactDOM from 'react-dom';
import 'antd/dist/antd.css';
import './index.css';
import { Table, Input, Button, Popconfirm, Form } from 'antd';
const EditableContext = React.createContext(null);

const InputText = (props) => {
  const test_sentence = '阿爸_你_愛';
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
        onChange={(e) => {
          setSent(e.target.value);
          const new_datas = e.target.value.match(re);
          props.combine(new_datas);
        }}
      ></Input>
      
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
        width: '20%',
      },
      {
        title: '拼音',
        dataIndex: 'spell',
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
    let initial_words = [
      {
        key: '0',
        pk: '阿爸',
        spell: 'a24 ba24',
        freq: '1034',
        class: 'n',
      },
      {
        key: '1',
        pk: '你',
        spell: 'ni11',
        freq: '6338',
        class: 'pron',
      },
      {
        key: '2',
        pk: '愛',
        spell: 'oi55',
        freq: '10387',
        class: 'n',
      },
    ];
    this.state = {
      dataSource: [...initial_words],
      words: [...initial_words],
      count: 2,
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
    const { dataSource } = this.state;
    const priority = { '': 0, adj: 1, n: 2, pron: 3, adv: 4, v: 5 };
    let words = [];
    let new_word = {};
    let found = false;
    for (let i = 0; i < new_words.length; i++) {
      let result = dataSource.filter((w) => w.pk === new_words[i]);
      if (result.length !== 0) {
        result[0].key = `${i}`;
        words.push(result[0]);
      } else {
        found = false;
        new_word = { key: `${i}`, pk: '', spell: '', freq: '0', class: '' };
        for (let j = 0; j < dataSource.length; j++) {
          if (new_words[i].search(dataSource[j].pk) !== -1) {
            found = true;
            new_word.pk += dataSource[j].pk;
            new_word.spell += `${dataSource[j].spell} `;
            if (priority[new_word.class] < priority[dataSource[j].class]) {
              new_word.class = dataSource[j].class;
            }
          } else if (found) {
            break;
          }
        }
        words.push(new_word);
      }
    }
    // console.log(words);
    this.setState({ words: [...words] });
  };

  render() {
    const {words} = this.state;
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

    return (
      <div style={{
        width: '60vw',
        margin: '10px auto',
        minHeight: '600px',
      }}>
        <InputText
          combine={(new_words) => {
            this.handleCombine(new_words);
          }}
        />

        <Table
          components={components}
          rowClassName={() => 'editable-row'}
          bordered
          dataSource={words}
          columns={columns}
        />
      </div>
    );
  }
}

export default EditableTable;