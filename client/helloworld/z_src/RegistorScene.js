import React,{Component, PropTypes} from 'react';
import {
  Text,
  TextInput,
  Image,
  View,
  TouchableHighlight,
  Alert,
}from 'react-native';

export default class RegistorScene extends Component{

  render(){
    return(
      <View style={{alignItems: 'center'}}>{/*整个页面*/}
        {/*title*/}
        <View style={{width:380,alignItems: 'stretch', flexDirection:'row'}}>
          <Text
            style={{width:40,fontSize:18,  marginTop:3, marginLeft:5}}
            onPress={this.props.onBack}
            >
            返回
          </Text>
          <Text style={{width:300,textAlign:'center' ,fontSize:24}}>用户注册</Text>
        </View>
      </View>
    )
  }
}
RegistorScene.propTypes = {
  onBack:PropTypes.func.isRequired,
}
