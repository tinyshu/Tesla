/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 * @flow
 */

import React, { Component } from 'react';
import {
  AppRegistry,
  View,
  Text,
  Navigator,
  StatusBar,
} from 'react-native';
import LoginModule from './LoginModule';


export default class Start extends Component {
  render() {
    //初始化一些config信息
    let Config = require('./Config');
    Config.screen.init();
    return (
        <Navigator//这一部分只负责登录相关等几个界面等导航
          initialRoute={{ sceneName: 'login_scene', statusBarHidden:true, index: 0 }}
          renderScene={(route, navigator) =>
            <LoginModule
              sceneName={route.sceneName}
              statusBarHidden={route.statusBarHidden}
              //忘记密码
              onForget={ () => {
                const nextIndex = route.index + 1;
                navigator.push({
                  sceneName: 'forget_scene',
                  index: nextIndex,
                });
              }}
              //注册
              onRegistor = {()=>{
                const nextIndex = route.index + 1;
                navigator.push({
                  sceneName: 'registor_scene',
                  index: nextIndex,
                });
              }}

              // Function to call to go back to the previous scene
              onBack={() => {
                if (route.index > 0) {
                  navigator.pop();
                }
              }}
            />
          }
        >
        </Navigator>
    )
  }
}
