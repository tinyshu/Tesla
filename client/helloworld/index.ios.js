/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 * @flow
 */

import React, { Component } from 'react';
import {
  AppRegistry,
} from 'react-native';
import Start from './z_src/Start';
class helloworld extends Component {
  render() {
    return (
        <Start />
    )
  }
}

AppRegistry.registerComponent('helloworld', () => helloworld);
