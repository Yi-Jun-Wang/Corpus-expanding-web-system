import Public_page from './Public_page'
import Private_page from './Private_page';
import Login from './Login'
import Logout from './Logout'
import Home from './Home'
import { useState } from 'react'
import { Routes, Route } from 'react-router-dom';

function App() {
    const [auth, setAuth] = useState(false);
    return (
        <Routes>
            <Route path="/" element={<Home auth={auth}/>}>
                <Route path="/" element={<Public_page auth={auth}/>} />
                <Route path="private" element={<Private_page auth={auth}/>} />
                <Route path="login" element={<Login auth={auth} onChange={(value)=>{setAuth(value)}}/>} />
                <Route path="logout" element={<Logout onChange={value=>{setAuth(value)}}/>} />
            </Route>
        </Routes>
    );
}

export default App
