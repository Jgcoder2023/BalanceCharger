<template>
	<view class="bgview">
		<view class="one_line">
			<view class="l_box">
				<view class="box">
					<view class="big_text">{{ AppData.OutV}}</view>
					<view class="small_text">输出电压</view>
				</view>
			</view>
			<view class="r_box">
				<view class="box">
					<view class="big_text">{{AppData.OutA}}</view>
					<view class="small_text">输出电流</view>
				</view>
			</view>
		</view>
		

		
		<view class="two_line">
			<text>当前工作模式：{{modelTitle}}</text>
		</view>
		
		<view class="three_line">
			<view class="l_box">
				<view class="box">
					<view class="big_text">{{AppData.SetInV}}</view>
					<view class="small_text">设置输入电压</view>
				</view>
			</view>
			<view class="r_box">
				<view class="box">
					<view class="big_text">{{AppData.SetOutV}}</view>
					<view class="small_text">设置输出电压</view>
				</view>
			</view>
		</view>
		
		
		<view class="five_line">
			<button type="default" @click="edit_value">远程修改电压</button>
		</view>
		
	</view>
</template>

<script>
	import com from "/common/com.js"
	export default {
			data(){
				return{
					AppData :com.Data,
					modelTitle:""
				}
			},
			onLoad(){
				uni.$on('AppData',(data)=>{
					this.AppData = data;
					switch (data.SetOutKey){
						case '4':
							this.modelTitle = "电池充电模式（1节电池）";
							break;
						case '9':
							this.modelTitle = "电池充电模式（2节电池）";
							break;
						case '14':
							this.modelTitle = "电池充电模式（3节电池）";
							break;
						case '19':
							this.modelTitle = "电池充电模式（4节电池）";
						break;
						default:
							this.modelTitle = "电压源模式";
						break
					}
					this.$forceUpdate();
				});
			},
			methods: {
				edit_value() {
					uni.navigateTo({
						url: 'edit'
					});
				}
			}
		}
</script>

<style>
	.bgview{
		width: 750rpx;
		height: 1240rpx;
		background: linear-gradient(180deg, #6579FF 0%, #8EB7FD 41%, #F0F5FF 100%);
		border-radius: 0rpx 0rpx 0rpx 0rpx;
	}
	.one_line{
		display: flex;
		flex-direction: row;
		flex-wrap: nowrap;
		justify-content: space-between;
		height: 330rpx;
	}
	.l_box{
		margin-left: 96rpx;
	}
	.r_box{
		margin-right: 96rpx;
	}
	.l_box,.r_box{
		margin-top: 80rpx;
		width: 244rpx;
		height: 188rpx;
		background: #FFFFFF;
		box-shadow: 0rpx 3rpx 6rpx 1rpx rgba(0,0,0,0.16);
		border-radius: 21rpx 21rpx 21rpx 21rpx;
		opacity: 1;
		
		display: flex;
		align-items:center;
		justify-content: center;
	}
	
	.big_text{
		width: 240rpx;
		text-align: center;
		font-size: 63rpx;
		font-weight: bold;
		color: #283E65;
	}
	.small_text{
		width: 240rpx;
		text-align: center;
		font-size: 29rpx;
		font-weight: 400;
		color: #2C3A53;
		margin-top: 10rpx;
	}
	
	.two_line{
		width: 700rpx;
		height: 48rpx;
	}
	.two_line text{
		font-size: 31rpx;
		font-weight: bold;
		color: #283E65;
		margin-left: 30rpx;
	}
	.three_line{
		display: flex;
		flex-direction: row;
		flex-wrap: nowrap;
		justify-content: space-between;
		height: 300rpx;
	}

	.three_line .l_box,.three_line .r_box{
		margin-top: 40rpx;
	}


	.four_line{
		display: flex;
		flex-direction: row;
		flex-wrap: nowrap;
		justify-content: space-around;
		height: 320rpx;
	}
	.four_line image{
		width: 140rpx;
		height: 285rpx;
	}

	.four_line .ch_box{
		position: relative;
		text-align: center;
	}
	.four_line text{
		width: 140rpx;
		position: absolute;
		left: 0;
		right: 0;
		bottom: 23rpx;
		
		font-size: 26rpx;
		font-weight: bold;
		color: #283E65;
	}



	.five_line{
		height: 200rpx;
		margin-top: 33rpx;
	}
	
	
	.five_line button{
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