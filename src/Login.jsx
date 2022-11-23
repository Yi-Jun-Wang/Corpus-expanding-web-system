import 'antd/dist/antd.css';
import './App.css'
import React from 'react'
import { LockOutlined, UserOutlined, EyeInvisibleOutlined, EyeTwoTone } from '@ant-design/icons';
import { Button, Input, message } from 'antd';
import { Navigate } from 'react-router-dom';

class Login_interface extends React.Component {
    
    constructor(props) {
        super(props);

        this.state = {
            acc: '',
            pass: '',
            status: '',
        };
    }
    
    handleClick = () => {
        const { onChange } = this.props;
        const data = {...this.state};
        axios.post(`http://10.10.8.42:5000/login`, data)
        .then((response) => {
            console.log('login successfully.');
            if (onChange != undefined){
                onChange(true);
            }
        })
        .catch((e) => {
            console.log(e);
            this.setState({status:'error'})
            message.error("Incorrect username or password.");
            onChange(false);
        })
    }

    render(){
        if (this.props.auth){
            return <Navigate to="/private" />;
        }
        return(
            <div style={{
                display: 'flex',
                justifyContent: 'center',
                height: '60vh',            
                alignItems: 'center'
            }}>
                <div style={{
                    display: 'flex',
                    justifyContent: 'center',
                    position: 'relative'
                }}>
                    <div style={{
                        position: 'relative'
                        }}>
                    <Input
                        style={{
                            height: '35px'
                        }}
                        status={this.state.status}
                        placeholder='Account'
                        onChange={(e) => {this.setState({acc: e.target.value});}}
                        prefix={<UserOutlined/>} />
                    <Input.Password 
                        style={{
                            height: '35px'
                        }}
                        status={this.state.status}
                        placeholder='Password'
                        onChange={(e) => {this.setState({pass: e.target.value});}}
                        prefix={<LockOutlined/>}
                        iconRender={(visible) => (visible ? <EyeTwoTone /> : <EyeInvisibleOutlined />)} />
                    </div>
                    <div style={{
                        position: 'absolute',
                        top: '-15px',
                        left: '430px'
                    }}>
                    <Button
                        style={{
                            height: '70px'
                        }}
                        type="primary"
                        onClick={this.handleClick}
                    >登入
                    </Button>
                    </div>              
                </div>
            </div>
        );
    }
}

export default Login_interface