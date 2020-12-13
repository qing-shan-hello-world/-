var app = getApp()


Page({
  data: {
    height1: 0,
    height2: 0,
    height3: 0,
    array: ['未选择', '1小时', '2小时', '3小时'],
　  index: 0,
  },
  onLoad(){
    this.setData({
      hum1: app.globalData.humidity1.datapoints[0].value,
      time1:app.globalData.humidity1.datapoints[0].at,
      hum2: app.globalData.humidity1.datapoints[1].value,
      time2:app.globalData.humidity1.datapoints[1].at,
      hum3: app.globalData.humidity1.datapoints[2].value,
      time3:app.globalData.humidity1.datapoints[2].at,
    })
  },
  onPullDownRefresh: function () {
    console.log('onPullDownRefresh', new Date())
  },



  bindPickerChange: function (e) {
    　　console.log('picker发送选择改变，携带值为', e.detail.value)
    　　this.setData({
    　　　index: e.detail.value
    　　})
    this.setData({
      key1: e.detail.value,
      key2: app.globalData.key1,
    })
    if(this.data.value1!=''){
      this.setData({
        disabled: false
      })
    }
    var that=this
    let deviceid = "645459120"
    let apikey = "OvtuYOhNenw5o0afM=TqYx3=DRI="
    let data={
      "datastreams": [
        {"id": "temp1","datapoints":[{"value": that.data.key1}]}
      ]

    }
    wx.request({
      url: "https://api.heclouds.com/devices/" + deviceid + "/datapoints",
      method:'POST',
      header:{
        "content-type": 'application/json',
        "api-key": apikey
      },
      data:JSON.stringify(data),
      success(res){
        console.log("更新数据成功",res)
      },
      fail: function(res){
        wx.showToast({ title: '系统错误' })
      },
      complete:function(res){
        wx.hideLoading()
      }
    })
    },



})