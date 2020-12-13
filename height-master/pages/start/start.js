 // start.js

Page({

  /**
   * 页面的初始数据
   */
  data: {
    opacity: 0.4,
    disabled: true,
    threshold: 0,
    rule: 'up',
    items: [
      { name: 'up', value: '怕热', checked: 'ture' },
      { name: 'down', value: '怕冷' },
    ]
  },

  getDataFromOneNet: function () {
    //从oneNET请求我们的自动身高测量仪的数据
    const requestTask = wx.request({
      url: 'http://api.heclouds.com/devices/645459120/datapoints?datastream_id=Temperature,Humidity&limit=3',
      header: {
        'content-type': 'application/json',
        'api-key': 'OvtuYOhNenw5o0afM=TqYx3=DRI='
      },
      success: function (res) {
        console.log(res.data)
        //拿到数据后保存到全局数据
        var app = getApp()
        app.globalData.temperature1 = res.data.data.datastreams[0],
        app.globalData.humidity1 = res.data.data.datastreams[1],
        console.log(app.globalData)
        wx.navigateTo({
          url: '../page1/page1',
        })
      },

      fail: function (res) {
        console.log("fail!!!")
      },

      complete: function (res) {
        console.log("end")
      }
    })
  },

  change: function (e) {
    //当有输入时激活发送按钮，无输入则禁用按钮
    if (e.detail.value != "") {
      this.setData({
        threshold: e.detail.value,
        opacity: 1,
        disabled: false,
      })
    } else {
      this.setData({
        threshold: 0,
        opacity: 0.4,
        disabled: true,
      })
    }
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {

  },

  /**
   * 生命周期函数--监听页面初次渲染完成
   */
  onReady: function () {

  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow: function () {

  },

  /**
   * 生命周期函数--监听页面隐藏
   */
  onHide: function () {

  },

  /**
   * 生命周期函数--监听页面卸载
   */
  onUnload: function () {

  },

  /**
   * 页面相关事件处理函数--监听用户下拉动作
   */
  onPullDownRefresh: function () {

  },

  /**
   * 页面上拉触底事件的处理函数
   */
  onReachBottom: function () {

  },

  /**
   * 用户点击右上角分享
   */
  onShareAppMessage: function () {

  }
})