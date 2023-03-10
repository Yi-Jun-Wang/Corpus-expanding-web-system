import 'antd/dist/antd.css';
import './App.css'
import { Outlet, Link } from 'react-router-dom'
import { message } from 'antd';
const Home = (props) => {
    return (
        <div className="App">
          <div id="Top-area" className="big-bg">
              <header className="page-header wrapper">
                  <h1>
                    <Link to="/">
                    <img className="logo" src="src/images/NYCU.png" alt="陽明交通大學" />
                    </Link>
                  </h1>
                  <nav>
                      <ul className="main-nav">
                          <li>
                            <Link to={"/wdcomb"}>
                                {props.auth ? '新增詞句' : <span onClick={() => {message.warning("登入後才可訪問該頁面");}}>新增詞句</span>}
                            </Link>
                          </li>
                          <li><Link to={props.auth ? "/logout" : "/login"}>{props.auth ? 'Logout' : 'Login'}</Link></li>
                      </ul>
                  </nav>
              </header>

              <div className="wrapper">
                  <h2 className="page-title">客語詞庫擴充系統</h2>
              </div>
          </div>

          <Outlet />

          <footer>
              <div className="wrapper">
                  <p><small>&copy; 2022 Jimmy</small></p>
              </div>
          </footer>
        </div>
    );
}

export default Home;