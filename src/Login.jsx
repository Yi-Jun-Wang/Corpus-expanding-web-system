import 'antd/dist/antd.css';
import './App.css'
import React from 'react'
import { LockOutlined, UserOutlined, EyeInvisibleOutlined, EyeTwoTone } from '@ant-design/icons';
import { Button, Input, message } from 'antd';
import { Navigate } from 'react-router-dom';

// const api_url = 'https://10.10.8.42:5000';
const api_url = 'https://127.0.0.1:5000';
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
        axios.post(`${api_url}/login`, data)
        .then((response) => {
            message.success("Login successfully.")
            console.log(response);
            sessionStorage.setItem("token", response.data['access_token']);
            if (onChange != undefined){
                onChange(true);
            }
        })
        .catch((e) => {
            message.error("Incorrect username or password.");
            console.log(e);
            this.setState({status:'error'})
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
                        onPressEnter={this.handleClick}
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