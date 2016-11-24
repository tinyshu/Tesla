import {Dimensions,PixelRatio, Platform, NativeModules} from 'react-native';
const {StatusBarManager} = NativeModules;
//下面我们以iphone6p的尺寸数据作为标准尺寸，没有实际意义，只是一个参考值，用来计算其他数据
//目前的设计方案是ios和android统一都不渲染statusbar后面的部分
//TODO:如果后续ios希望在statusbar下也进行渲染，那么则需要在ios平台下把bar的高度也加到height里面，同时将margin改为padding
exports.screen = {
  /**
  我们根本没必要去关心实际的px是多少，也没必要去算，
  因为算出来也有可能是近似出来的。所以我们就选取当前1920*1080，
  dp(ios-pt)为414*736，来做为一个基准值进行设计，不去关心其他东西。去除顶部bar的高度20，剩余414*716，换成像素 1868*1080
  PS:下面的dp只是一个统称，其实在ios下是pt,安卓平台是dp
  */
  statusbar_height:Platform.OS === 'ios' ? 20:StatusBarManager.HEIGHT,
  view_padding_top:Platform.OS === 'ios' ? 20:0,//这个是android平台显示上面statusbar
  // view_padding_top:Platform.OS === 'ios' ? 20:StatusBarManager.HEIGHT,//这个是android平台statusbar融入app
  std_height_dp:716,//716-20
  std_width_dp:414,
  cur_pixel_ratio:PixelRatio.get(),
  cur_height_dp_with_statusbar:0,
  cur_height_dp_without_statusbar:0,//注意这个API在IOS平台读出来的是pt，在Android平台读出来的是dp
  cur_width_dp:Dimensions.get('window').width,

  cur_height_ratio_dp:1,//当前屏幕高度跟标准高度的比例:dp
  cur_width_ratio_dp:1,//当前屏幕宽度跟标准宽度的比例:dp
  //初始化一些需要计算的信息
  init : function(){
    this.cur_height_dp_without_statusbar = Dimensions.get('window').height - this.statusbar_height;
    this.cur_height_dp_with_statusbar = this.cur_height_dp_without_statusbar + this.statusbar_height;
    this.cur_height_ratio_dp = this.cur_height_dp_without_statusbar / this.std_height_dp;
    this.cur_width_ratio_dp = this.cur_width_dp / this.std_width_dp;
  },
  //获得一个std控件在当前屏幕下的高度dp
  getHeightDp:function(stdH){
    return stdH*this.cur_height_ratio_dp;
  },
  //获得一个std控件在当前屏幕下的宽度dp
  getWidthDp:function(stdW){
    return stdW*this.cur_width_ratio_dp;
  },
  //获得一个std字体大小在当前屏幕下的大小
  getFontSize:function(stdSize){
    if(this.cur_height_ratio_dp < this.cur_width_ratio_dp)
    {
      return stdSize*this.cur_height_ratio_dp;
    }
    else {
      return stdSize*this.cur_width_ratio_dp;
    }
  },
};
