import React, {Component, PropTypes} from 'react';
import {View, Text, TouchableHighlight,StatusBar} from 'react-native';

import LoginScene from './LoginScene';
import RegistorScene from './RegistorScene';
//这个是用来控制登录模块的几个页面的，包括登录，注册，找回密码等
export default class LoginModule extends Component
{
	render(){
		var curScene;
		switch(this.props.sceneName )
		{
			case 'login_scene':
				curScene = <LoginScene
					onForget = {this.props.onForget.bind(this)}
					onRegistor = {this.props.onRegistor.bind(this)}
					/>;
				break;
		case 'registor_scene':
			curScene = <RegistorScene
				onBack = {this.props.onBack}
			/>
			break;
		case 'forget_scene':
			curScene = <Text>forget_scene</Text>
			break;
		default:
			curScene = <Text>No Scene </Text>
			break;
		}
		return(
			<View>
				{/*这个保证ios和android平台都是沉浸式,但是妈的在1+上貌似不奏效，fuck这么多厂商*/}
				{/*<StatusBar translucent={true} backgroundColor={'transparent'}/ >*/}
				{curScene}
			</View>
		)
	}

}

LoginModule.propTypes = {
	sceneName:PropTypes.string.isRequired,
	onForget:PropTypes.func.isRequired,
	onRegistor:PropTypes.func.isRequired,
	onBack:PropTypes.func.isRequired,
};
