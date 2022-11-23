import {useEffect} from 'react'
import { Navigate } from 'react-router-dom';
const Logout = (props) => {
    useEffect( () => {
        props.onChange(false);
    });
    return <Navigate to="/" />;
}
export default Logout;