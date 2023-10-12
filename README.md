# wheelbarrow

第十八届全国大学生智能汽车大赛华东赛区独轮项目

主要文件结构说明  

user文件夹为主要逻辑功能代码  

​	---ano 黎明上位机相关  
​	---attitude 陀螺仪数据获取,角度姿态解算  
​	**---balance  电机控制,保持直立控制等** 
  ---CAMERA   图像数据预处理  
  ---cpu0_main 0核主线程所在   控制核心   
  ---cpu1_main 1核主线程所在   图像处理核心   
  **---image      图像处理处理**  
  ---isr        中断处理  
  ---ImageProcess 获取图像中边线  

直立效果展示:https://www.bilibili.com/video/BV1xV411u7w5/?spm_id_from=333.999.0.0
识别效果展示:https://www.bilibili.com/video/BV1BW4y1d7fL/?spm_id_from=333.999.0.0
