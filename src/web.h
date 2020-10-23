#ifndef _WEB_h
#define _WEB_h

const char cType_HTML[] PROGMEM = "text/html";
const char cType_PLAIN[] PROGMEM = "text/plain";
const char cType_JSON[] PROGMEM = "application/json";

#define COLOR_HIGH  "red"
#define COLOR_LOW   "MediumSlateBlue"

const char T_zeroLevel[] PROGMEM = "zl";
const char T_voltOffset[] PROGMEM = "vo";

const char paramsJSON[] PROGMEM = "{\"params\":{\"zl\":\"%.3f\",\"vo\":\"%d\"}}";
const char valuesJSON[] PROGMEM = "{\"values\":{\"id0\":{\"v\":\"%.3f\",\"s\":\"white\"},\"id1\":{\"v\":\"%.3f\",\"s\":\"white\"},\"id2\":{\"v\":\"%.3f\",\"s\":\"%s\"}}}";



/*
GET /states
GET /params
POST /params
GET /restore
*/

/*
{
    "params":{
        "zl": "0.0",
        "vo": "0.0"
    }
}
zl - Zero Level
vo - Voltage Offset
*/

/*
{
    "values":{
        "id0":{
            "v":"0.0",
            "s":"white"
        },
        "id1":{
            "v":"0.0",
            "s":"white"
        }
        "id2":{
            "v":"0.0",
            "s":"white"
        }
    }
}
id1 - kPa
id1 - Volt
id2 - Bar
v - value
s - style
*/

const char HTML_INDEX_PAGE[] PROGMEM = "<!DOCTYPE html><html><meta name='format-detection' content='telephone=no'><meta charset='UTF-8'><meta name='viewport' content='width=device-width,initial-scale=1,user-scalable=no'>"
                                       "<script>function g(a){return document.getElementById(a)};function getParams(){var xhr=s('/params');xhr.send();xhr.onreadystatechange=function(e){if(xhr.readyState===4&&xhr.status===200){var data=JSON.parse(xhr.responseText)['params'];Object.keys(data).forEach(function(key){g(key).value=data[key];})}};};function s(u,m='GET'){const xhr=new XMLHttpRequest();xhr.open(m,u);return xhr;};let timerId=setTimeout(function run(){var xhr=s('/states');xhr.send();xhr.onreadystatechange=function(e){if(xhr.readyState===4&&xhr.status===200){var data=JSON.parse(xhr.responseText)['values'];Object.keys(data).forEach(function(key){g(key).innerHTML=data[key].v;g(key).style.color=data[key].s;})}};timerId=setTimeout(run,500);},500);function VisibleSettings(){var setClass=document.getElementsByClassName('settings')[0];setClass.style.visibility=setClass.style.visibility==='visible'?'hidden':'visible';};function setValue(){g('zl').value=g('id0').innerHTML;};function setParams(event){var xhr=s('/params','POST');xhr.onreadystatechange=function(){if(xhr.readyState>3&&xhr.status==200){console.log(xhr.responseText);}};xhr.setRequestHeader('X-Requested-With','XMLHttpRequest');xhr.setRequestHeader('Content-Type','application/x-www-form-urlencoded');xhr.send('zl='+g('zl').value+'&vo='+g('vo').value);};function restore(){var xhr=s('/restore');xhr.responseType='json';xhr.onload=function(){var params=xhr.response['params'];Object.keys(params).forEach(function(key){g(key).value=params[key];})};xhr.send();};</script>"
                                       "<head><meta charset='utf-8'><meta http-equiv='X-UA-Compatible' content='IE=edge'>"
                                       "<style>body{background-color:#000;text-align:center;font-family:verdana}.main{text-align:left;display:inline-block;min-width:260px;max-width:500px;color:white}table{width:100%;margin:auto}td{text-align:center}.tc0{text-align:right;width:70px}.tc1{text-align:center}.tc1.bar{color:MediumSlateBlue;font-size:150%}a{font-weight:700}button,input[type='submit']{cursor:pointer;border:1px;background-color:#1fa3ec;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;margin:2px}input{border-radius: .3rem;width:100%;margin:2px;box-sizing:border-box;text-align:center}.settings{box-sizing:border-box;visibility:hidden;padding:10px;text-align:left}input[type='button']{cursor:pointer;border:1px;background-color:#1fa3ec;color:#fff}label{margin:10px;color:#fff}.btn.blue{background-color:#1fa3ec;color:#fff}.btn.red{background-color:#ec1f1f;color:#fff}.btn.grey{background-color:#666;color:#fff}.btn.alone{margin-top:15px;margin-bottom:15px}#btnzl{width:100px}</style>"
                                       "</head>"
                                       "<body onload='getParams();'><div class='main'> <br>Pressure:<table><tbody><tr><td class='tc0'>kPa:</td><td class='tc1'><a id='id0'>0.0</a></td></tr><tr><td class='tc0'>V:</td><td class='tc1'><a id='id1'>0.0</a></td></tr><tr><td class='tc0'>Bar:</td><td class='tc1 bar'><a id='id2'>0.0</a></td></tr></tbody></table></div><div> <br /><button id='btnSettings' onClick='event.preventDefault();getParams();VisibleSettings();'>Settings</button><div class='settings' id='settings'> <br /><label for='vo'>Offset (mV):</label><input id='vo' name='vo'> <br /> <label for='zl'>Zero level (kPa):</label><div style='display:flex;width: 100%;'> <input id='zl' name='zl' style='width:100%;'> <input id='btnzl' type='button' onClick='setValue();' value='<<<'></div> <br /><button id='btnU' class='btn blue' style='visibility: collapse;' onClick='event.preventDefault();'>Upgrade SW (OTA)</button> <br /><button id='btnR' class='btn blue' onClick='event.preventDefault();restore();'>Restore to default</button> <br /><button id='btnS' class='btn red alone' onClick='event.preventDefault();setParams(this);VisibleSettings();'>Save</button></div></div></body></html>";

#endif