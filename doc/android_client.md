#
- https://discourse.lynckia.com/t/licode-android-new-client/750/2
##

Have the same problem for K-GmbH/licodeAndroidClient six months ago, and then turn to make my own Licode-ErizoClientAndroid from referencing Licode-ErizoClientIOS and Android apprtc 18. And I’m very sorry for not opening this ErizoClientAndroid source due to our company’s policy. But I can describe my steps to porting Licode-ErizoClientIOS to Licode-ErizoClientAndroid:

Trace Licode-ErizoClientIOS to understand Licode signaling & WebRTC RTCPeerConnection flow, you have to trace Objective-C source code and will be better to have iOS device and developer account. (About one week)
Integrate Licode signaling channel for emitting(token, publish, subscribe) and receiving(signaling_message_erizo) event, tested well for Licode pre-v5.4 and Android Socket.io 1 client 1.0.0. (About one week)
1 v.s. 1 video call: Use prebuilt google-webrtc version 1.0.20849 and reference apprtc to integrate your WebRTC RTCPeerConnection logic into above signaling exchange, especially for SDP and ice candidate info.(About one week)
Multiple vide calls: Need to manage multiple RTCPeerConnection: one for publish and many for subscribe (About two weeks)
And will be better to understand licode architecture diagram first before you start
