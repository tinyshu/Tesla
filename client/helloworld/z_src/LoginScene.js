import React,{Component, PropTypes} from 'react';
import {
  Text,
  TextInput,
  Image,
  View,
  TouchableHighlight,
  Alert,
  Dimensions,
  PixelRatio,
  Platform,
}from 'react-native';
//登录按钮点击
const onLoginButtonPress = () => {
  Alert.alert('LoginButton has been pressed!');
};
var Config = require('./Config')

export default class LoginScene extends Component
{

  render() {
    let pic = {
          uri: 'https://facebook.github.io/react/img/logo_og.png'
        };
    return (
      <View style={{paddingTop:Config.screen.view_padding_top, height:Platform.OS === 'ios'?Config.screen.cur_height_dp_with_statusbar: Config.screen.cur_height_dp_without_statusbar, width:Config.screen.cur_width_dp,alignItems: 'center', backgroundColor:'green'}}>{/*整个页面*/}
        {/*LOGO*/}
        <Image
          source={pic}
          style={{width:Config.screen.cur_width_dp,height:Config.screen.getHeightDp(266),resizeMode:Image.resizeMode.contain, backgroundColor:'purple'}}
        />
        {/*帐号*/}
        <View style = {{backgroundColor:'yellow',flexDirection:'row', height: Config.screen.getHeightDp(70), paddingTop:Config.screen.getHeightDp(30), paddingLeft:Config.screen.getWidthDp(20)}}>
          <Text style = {{backgroundColor:'red',color:'black', fontSize:Config.screen.getFontSize(24)}}>帐号</Text>
          <TextInput
            style = {{backgroundColor:'white',padding:0,fontSize:Config.screen.getFontSize(14), height:Config.screen.getHeightDp(40), width:Config.screen.getWidthDp(250), marginLeft:Config.screen.getWidthDp(10)}}
            placeholder = "请输入您的手机号码"
            placeholderFontSize = {11}
            underlineColorAndroid = 'transparent'
            maxLength = {11}
            keyboardType = {'phone-pad'}
            blurOnSubmit = {true}
          />
        </View>
        {/*密码*/}
        <View style = {{flexDirection:'row', height: Config.screen.getHeightDp(70), paddingTop:Config.screen.getHeightDp(30), paddingLeft:Config.screen.getWidthDp(20)}}>
          <Text style = {{color:'black', fontSize:Config.screen.getFontSize(24), marginTop:Config.screen.getHeightDp(7)}}>密码</Text>
          <TextInput
            style = {{padding:0, fontSize:Config.screen.getFontSize(24), height:Config.screen.getHeightDp(40), width:Config.screen.getWidthDp(250), marginLeft:Config.screen.getWidthDp(10)}}
            placeholder = {"请输入您的密码"}
            secureTextEntry = {true}
            maxLength = {16}
            keyboardType = {'default'}
          />
        </View>
        {/*登录按钮*/}
        <TouchableHighlight
              style = {{width:Config.screen.getWidthDp(340), height:Config.screen.getHeightDp(42),marginTop:Config.screen.getHeightDp(20), alignItems: 'center', borderRadius:Config.screen.getHeightDp(8), backgroundColor:'deepskyblue'}}
							underlayColor={'deepskyblue'}
							activeOpacity={0.5}
							onPress={onLoginButtonPress}
						>
            <Text style={{ fontSize:Config.screen.getFontSize(20), textAlign:'center', color:'white',marginTop:Config.screen.getHeightDp(11)}} numberOfLines={ 1 }>
              登录
            </Text>

        </TouchableHighlight>
        {/*忘记密码*/}
        <Text
          style = {{height:Config.screen.getHeightDp(40), fontSize:Config.screen.getFontSize(18), marginTop:Config.screen.getHeightDp(15)}}
          onPress = {this.props.onForget}
        >
          忘记密码？
        </Text>
        {/*版本号*/}
        <Text
          style = {{height:Config.screen.getHeightDp(40),fontSize:Config.screen.getFontSize(18), marginTop:Config.screen.getHeightDp(15)}}
        >
          版本号：V1.0
        </Text>
        {/*注册*/}
        <TouchableHighlight
              style = {{width:Config.screen.getWidthDp(340), height:Config.screen.getHeightDp(30),marginTop:Config.screen.getHeightDp(20), alignItems: 'center', borderRadius:4, backgroundColor:'aliceblue'}}
              underlayColor={'aliceblue'}
              activeOpacity={0.5}
              onPress={this.props.onRegistor}
            >
            <Text style={{ fontSize:Config.screen.getFontSize(18), textAlign:'center', color:'black',marginTop:Config.screen.getHeightDp(5)}} numberOfLines={ 1 }>
              没有帐号？请注册
            </Text>

        </TouchableHighlight>
        <View style={{height:Config.screen.getHeightDp(88), backgroundColor:'red'}} >
        <Text>RED</Text>
        </View>
      </View>
    );
  }
}

LoginScene.propTypes = {
  onForget:PropTypes.func.isRequired,
  onRegistor:PropTypes.func.isRequired,
};
