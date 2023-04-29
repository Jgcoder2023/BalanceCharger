
const url = "http://xxxxx.com/getValue";


var is_updata = true;

//输出电压数据的定义
const SetOutV = ["Min","2V","3V","4V","4.2V","5V","6V","7V","8V","8.4V","9V","10V","11V","12V","12.6V","13V","14V","15V","16V","16.8V","17V","18V","18V","Max"];
//输入电压的定义
const SetInV = ["5V","9V","12V","15V","20V"];

const battery = ["电压源模式",'1节','2节','3节','4节'];

var inputSetVoltageIndex = 0;
var outSetVoltageIndex = 0;


var Data = {
	  "SetInV":"",
	  "SetOutV":"",
	  "SetOutKey":0,
	  "OutV":"",
	  "OutA":""
}

//其他页面获取数据
function GetData(){
	return Data;
}


//网络请求数据
function get_app_data(){
	if(!is_updata){
		return;
	}
	
	uni.request({
	    url: url,
	    data: {},
	    header: {},
	    success: (res) => {
			inputSetVoltageIndex = res.data.set_in_v;
			outSetVoltageIndex = res.data.set_out_v;
			Data.SetInV = SetInV[res.data.set_in_v];
			Data.SetOutV = SetOutV[res.data.set_out_v];
			Data.SetOutKey = res.data.set_out_v;
			Data.OutV = res.data.out_v.split(':')[1];
			Data.OutA = res.data.out_a.split(':')[1];
			uni.$emit("AppData",Data);
			return;
	    }
	});
	return;
}

setInterval(get_app_data, 1000);

//导出
export default {
	GetData,
	Data,
	SetOutV,
	SetInV,
	battery,
	is_updata,
	inputSetVoltageIndex,
	outSetVoltageIndex
}