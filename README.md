这个代码是我尝试使用python调用darknet版的YOLOv3程序
我尝试了几个方法都没有成功，这个代码是我尝试使用python调用darknet版的YOLOv3程序
（1）首先我尝试了用c++给python传mat数据
失败！因为darknet压根就没有提供mat的数据接口，好坑啊，为什么！

（2）然后我尝试了用c++给python传一个float*的指针，因为image的data数据就是float*
失败！python的拓展接口里面没有float*,没法直接传，因此得分装成结构体再强转，太麻烦，放弃吧

（2）最后我尝试了修改darknet的接口，希望提供一个mat_to_image的接口，但是又遇到了c调用c++接口的namespace问题，刚刚好我的电脑装的有事3.4.1版本的opencv，这一版opencv里面提供了c的接口，但是却不能用c调用，3.4.0的好像就可以，哇，自己被自己坑到了

所以我打算放弃这个方案，毕竟darknet不是一个用得太多的框架，接下来我打算转战tensorflow版的YOLOv3

最后解决方案 https://blog.csdn.net/weixin_44580210/article/details/89735760
