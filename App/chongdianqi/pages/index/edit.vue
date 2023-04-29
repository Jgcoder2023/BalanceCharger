<template>
	<view class="bgview">
		<view class="one_line">
			<view class="title">输入电压:</view>
			<picker class="input" @change="inputSetVoltageChange" :value="inputSetVoltageIndex" :range="inputSetVoltage">
				<view class="uni-input">{{inputSetVoltage[inputSetVoltageIndex]}}</view>
			</picker>
		</view>
		
		<view class="two_line">
			<view class="title">输出电压:</view>
			<picker class="input"  @change="outSetVoltageChange" :value="outSetVoltageIndex" :range="outSetVoltage">
				<view class="uni-input">{{outSetVoltage[outSetVoltageIndex]}}</view>
			</picker>
		</view>
		
		<view class="three_line">
			<view class="title">电池节数:</view>
			<view class="battery">{{battery[batteryIndex]}}</view>
		</view>
		
		<view class="four_line">
			<button type="default" @click="edit_value">修改参数</button>
		</view>
	</view>
</template>

<script>
import com from "/common/com.js"
export default{
	data(){
		return{
			is_disabled:true,
			inputSetVoltage: [],
			inputSetVoltageIndex: 0,
			outSetVoltage: [],
			outSetVoltageIndex: 0,
			battery:[],
			batteryIndex:0
		}
	},
	onLoad(){
		com.is_updata = false;
		this.inputSetVoltage = com.SetInV;
		// this.inputSetVoltageIndex = com.inputSetVoltageIndex;
		this.outSetVoltage = com.SetOutV;
		// this.outSetVoltageIndex = com.outSetVoltageIndex;
		this.battery = com.battery;
	},
	methods:{
		edit_value: function(){
			//发起网络请求
			uni.request({
			    url: 'http://xxxxxx/SetValue?setValue=' + this.inputSetVoltageIndex + '_' + this.outSetVoltageIndex, 
				method:'GET',
			    success: (res) => {
					uni.showToast({
						title: '操作成功',
						icon:'success',
						success() {
							com.is_updata = true;
							uni.reLaunch({
								url: "index"
							});
							
						}
					});
				},
			});
		},
		//设置输入电压
		inputSetVoltageChange: function(e) {
		    this.inputSetVoltageIndex = e.detail.value;
		},
		//设置输出电压
		outSetVoltageChange:function(e){
			this.outSetVoltageIndex = e.detail.value;
			switch (this.outSetVoltageIndex){
				case 4:
					this.batteryIndex = 1;
					break;
				case 9:
					this.batteryIndex = 2;
					break;
				case 14:
					this.batteryIndex = 3;
					break;
				case 19:
					this.batteryIndex = 4;
				break;
				default:
					this.batteryIndex = 0;
				break
				
			}
		},
	},
}
</script>

<style>
	.bgview{
		width: 750rpx;
		height: 1240rpx;
		background: linear-gradient(180deg, #6579FF 0%, #8EB7FD 41%, #F0F5FF 100%);
		border-radius: 0rpx 0rpx 0rpx 0rpx;
	}
	.one_line,.two_line,.three_line{
		height: 120rpx;
		display: flex;
		justify-content: flex-start;
		flex-wrap: nowrap;
		flex-direction: row;
		align-items: center;
	}
	
	
	.title{
		width: 140rpx;
		font-size: 31rpx;
		font-weight: bold;
		color: #FFFFFF;
		margin-left: 52rpx;
		margin-right: 27rpx;
	}
	
	
	.three_line .battery{
		color: #FFFFFF;
		font-weight: bold;
	}
	
	.input{
		width: 384rpx;
		height: 69rpx;
		background: #FFFFFF;
		border-radius: 14rpx 14rpx 14rpx 14rpx;
		opacity: 1;
		border: 1rpx solid #EFF4FF;
	}
	
	.disabledInput{
		width: 384rpx;
		height: 69rpx;
		background: #d3d3d3;
		border-radius: 14rpx 14rpx 14rpx 14rpx;
		opacity: 1;
		border: 1rpx solid #d3d3d3;
	}
	
	.uni-input{
		margin:12rpx 0 0rpx 20rpx;
		height: 69rpx;
		color: #000;
	}
	
	
	
	.four_line{
		height: 200rpx;
		margin-top: 50rpx;
	}
	
	
	.four_line button{
		width: 530rpx;
		height: 95rpx;
		background: #409EFF;
		box-shadow: 0rpx 3rpx 6rpx 1rpx rgba(180, 180, 180, 0.2);
		border-radius: 48rpx 48rpx 48rpx 48rpx;
		opacity: 1;
		
		color: #ffffff;
		font-size: 38rpx;
	}
</style>