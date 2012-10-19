#pragma once

//CPP 中包含ffmpeg头文件时，需要使用 extern "C" 声明为C编译链接方式
#if 0
#  ifdef __cplusplus
extern "C" {
#  endif

#include <libavcodec\avcodec.h>  
#include <libavformat\avformat.h>  
#include <libswscale\swscale.h>  

#  ifdef __cplusplus
}
#  endif

#endif

//http://blog.sina.com.cn/s/blog_51396f890100o1yf.html
//http://www.cnblogs.com/billcoco/archive/2012/06/18/2553939.html -- FFMpeg写MP4文件例子分析
/*********************************************************************************************
* 缓冲区内存分配
*   1.av_image_alloc -- 推荐方式?
*   2.avpicture_get_size 获取大小后 av_malloc

* AVDictionary -- 一个键值对的数组，通常在获取或设置参数时使用
* 
* AVFrame
*   data/linesize -- FFMpeg内部以平面的方式存储原始图像数据，即将图像像素分为多个平面（R/G/B或Y/U/V），
*      data数组内的指针指向各个像素平面的起始位置，linesize数组则存放各个存贮各个平面的缓冲区的行宽
*   key_frame -- 该图像是否是关键帧
*   pict_type -- 该图像的编码类型：I(1)/P(2)/B(3)
*		
* AVStream -- 描述一个媒体流，其大部分信息可通过 avformat_open_input 根据文件头信息确定，其他信息可通过 avformat_find_stream_info 获取
*   典型的有 视频流、中英文音频流、中英文字幕流(Subtitle)，可通过 av_new_stream、avformat_new_stream 等创建
*   index -- 在AVFormatContext中流的索引，其值自动生成(AVFormatContext::streams[index])
*   nb_frames -- 流内的帧数目
*   r_frame_rate -- 
*   time_base -- 流的时间基准，是一个实数，该流中媒体数据的pts和dts都将以这个时间基准为粒度。通常，使用av_rescale/av_rescale_q可以实现不同时间基准的转换
* AVFormatContext -- 描述了一个媒体文件或媒体流的构成和基本信息，由 avformat_open_input 创建并初始化部分值，
*   但其他一些值(如 mux_rate、key 等)需要手工设置初始值，否则可能出现异常。
*   debug -- 调试标志
*   nb_streams/streams -- AVStream结构指针数组, 包含了所有内嵌媒体流的描述
* AVPacket -- 暂存解码之前的媒体数据（一个音/视频帧、一个字幕包等）及附加信息（解码时间戳、显示时间戳、时长等),
*   主要用于建立缓冲区并装载数据，
*   data/size/pos -- 数据缓冲区指针、长度和媒体流中的字节偏移量
*   flags -- 标志域的组合，1(AV_PKT_FLAG_KEY)表示该数据是一个关键帧, 2(AV_PKT_FLAG_CORRUPT)表示该数据已经损坏
*   destruct -- 释放数据缓冲区的函数指针，其值可为 [av_destruct_packet]/av_destruct_packet_nofree, 会被 av_free_packet 调用
*
* avformat_find_stream_info -- 获取必要的编解码器参数(如 AVMediaType、CodecID )，设置到 AVFormatContext::streams[i]::codec 中
* av_read_frame -- 从多媒体文件或多媒体流中读取媒体数据，获取的数据由 AVPacket 来存放
* av_seek_frame -- 改变媒体文件的读写指针来实现对媒体文件的随机访问，通常支持基于时间、文件偏移、帧号(AVSEEK_FLAG_FRAME)的随机访问方式，
* AVCodec
*   avcodec_find_decoder/avcodec_find_encoder -- 根据给定的codec id或解码器名称从系统中搜寻并返回一个AVCodec结构的指针
*   avcodec_alloc_context -- 
*   avcodec_open/avcodec_open2/avcodec_close -- 根据给定的 AVCodec 打开对应的Codec，并初始化 AVCodecContext/ 关闭Codec。
*   avcodec_alloc_frame -- 分配编解码时的 AVFrame 结构
*   avcodec_decode_video2 -- 解码一个视频帧，输入数据在AVPacket结构中，输出数据在AVFrame结构中。
*   avcodec_decode_audio4 -- 解码一个音频帧。输入数据在AVPacket结构中，输出数据在AVFrame结构中
*   
* 图像缩放和格式转换(sws_scale，早期是 img_convert)，如 RGB24 <-> YUV420P
*   转换算法的对比：http://www.cnblogs.com/acloud/archive/2011/10/29/sws_scale.html
*     SWS_FAST_BILINEAR 效率效果似乎都不错(默认推荐?); SWS_POINT 缩小时效率很高，但图像效果略差；
*   sws_getContext -- 根据输入、输出色彩空间的参数得到转换上下文
*   sws_scale -- 转换
*   
* 时间
*   设置编码后Packet的pts: pkt.pts = av_rescale_q(c->coded_frame->pts, c->time_base, video_st->time_base); 
*
* 文件操作
*   av_open_input_file -> av_find_stream_info(获取音视频流信息) -> 
*   avformat_write_header -> av_write_frame -> av_write_trailer
*   av_read_frame / av_write_frame
*********************************************************************************************/

/**********************************************************************************************
MainConcept -- 商业的编解码库，Camtasia 即在使用) http://www.mainconcept.com
*   TechSmith Screen Capture Codec(C:\Windows\System32\tsccvid.dll)
* 
* ffdshow -- 一套免费的编解码软件，封装成了DirectShow的标准组件。
*   使用 libavcodec library 以及其他各种开放源代码的软件包(如 DivX、)，可支持H.264、FLV、MPEG-4等格式视频
*  
* ffmpeg -- 开源且跨平台的音视频解决方案，是一套编解码的框架，具有采集、解码、流化等功能
*   http://www.ffmpeg.org/ <== 正式官网
*   http://www.ffmpeg.com.cn <== 中文网站，提供编译好的 FFmpeg SDK下载，有 FFmpeg SDK 开发手册
*   http://ffmpeg.zeranoe.com/builds/
*   http://bbs.chinavideo.org/viewthread.php?tid=1897&extra=page=1 <== Windows 下安装和编译
*   http://dranger.com/ffmpeg <== 英文的入门教材，但有些过时
* 模块
*   ffmpeg -i 输入文件 输出文件 -- 视频文件转换命令行工具,也支持经过实时电视卡抓取和编码成视频文件
*     例子： ffmpeg.exe -i snatch_1.vob -f avi -vcodec mpeg4 -acodec mp3 snatch.avi
*     -acodec codec		<== 指定音频编码，如 aac 
*     -an				<== 禁止audio
*     -aspect 16:9		<== 设置长宽比，如 4:3 或 16:9 等
*     -bf <int>			<== 使用B帧
*     -f format			<== 指定文件格式, 如 avi/mp4 等(可用格式可通过 -formats 查看)
*     -g groupSize		<== 设置GOP大小，如设置成 300 意味着29.97帧频下每10秒就有INTRA帧
*     -qblur <float>	<== 指定视频量化规模，0. ~ 1.0
*     -s WxH			<== 指定大小(宽x高)
*     -strict <int>     <== 指定对codec等的使用限制，实验性的codec(如mpeg4)默认是不启用的(Codec is experimental)，需要使用 -2 来启用
*     -t 持续时间		<== 指定时间长度，如 20.000 表示 20s
*     -threads <int>    <== 指定线程个数，默认为 auto ?
*     -vcodec codec		<== 指定视频编码，如 mpeg4/h264/msmpeg4v2 等(可用编码可通过 -codecs 查看)
*     -target type		<== 指定文件类型，如 vcd/dvd 等，很大程度上可以决定质量
*     -y                <== 直接覆盖输出文件，不再提示
*   ffserver -- 基于HTTP(RTSP正在开发中)用于实时广播的多媒体服务器.也支持时间平移(Time-Shifting)
*   ffplay -- 用 SDL和FFmpeg库开发的一个简单的媒体播放器(需要先安装 SDL 库才能编译)
*   libavcodec -- 包含了所有FFmpeg音视频编解码器的库.为了保证最优性能和高可复用性,大多数编解码器从头开发的
*   libavformat -- 包含了所有的普通音视频格式的解析器和产生器的库
*   libavutil -- 函数库，实现了CRC校验码的产生，128位整数数学，最大公约数，整数开方，整数取对数，内存分配，大端小端格式的转换等功能。
*   libswscale -- 
*   libpostproc -- 
* 
* 编译和调试(无法使用VC编译？)，可以用MSys+MinGW编译，但是编译出来的DLL是可以被VC使用的
*   http://blog.csdn.net/jszj/article/details/4028716  -- Windows 下编译
*   http://www.cnblogs.com/mcodec/articles/1659671.html -- 使用VC编译的方法
*   http://ffmpeg.org/trac/ffmpeg/wiki/How%20to%20setup%20Eclipse%20IDE%20for%20FFmpeg%20development -- 设置eclipse的环境
*     1.从 http://www.eclipse.org/downloads/ 下载 "Eclipse Classic"，或直接下载带 C/C++ 的版本
*     2.在线安装CDT插件: Help -> Install New Software -> http://download.eclipse.org/releases/indigo -> 搜索CDT
*     3.在线安装Git插件(http://www.vogella.de/articles/EGit/article.html), 
*     4.安装MinGW
*     4.Clone FFMpeg 的Git仓库: File->Import->Git->Projects From Git(git://source.ffmpeg.org/ffmpeg.git)
*     5.创建工程: File->New->Project-> C/C++ -> Makefile Project with Existing Code 
* 
*   http://ffmpeg.zeranoe.com/blog/  -- 自动下载和编译ffmpeg的脚本
*   编译选项
*     ./configure --enable-shared --enable-w32threads --disable-static --enable-memalign-hack --disable-yasm
*     --disable-debug			禁用调试信息,编译发布版本
*     --disable-mmx             
*     --disable-optimizations	禁止优化，一般用于调试版本
*     --disable-static			禁用静态库
*     --disable-stripping       一般用于调试版本
*     --disable-yasm            禁止使用yasm，否则可以安装 yasm后使用，(汇编编译器?)
*     --enable-gpl				遵循gpl协议,当使用 x264/xvid 等遵循gpl协议的库时必须指定
*     --enable-libfaac          支持aac(不是免费的),需要先到 faac 目录下编译, ./bootstrap<CR> ./configure xxx<CR> make
*     --enable-libx264			使用x264作为h264编码器,表示要使用第3方库x264,此时mingw include 和 lib内必须已经有x264.h和libx264.a
*                               ftp://ftp.videolan.org/pub/x264/snapshots/ 下载，然后 ./configure --enable-shared --enable-w32threads && make && make install
*                               
*     --enable-libxvid			启用 Xvid 编码器
*     --enable-libmp3lame		使用mp3lame作为mp3编码器，需要先到 lame 目录下编译（http://sourceforge.net/projects/lame/files/）
*     --enable-memalign-hack    启用内存对齐，Windows下必须
*     --enable-shared			编译动态库
*     --enable-small			启用文件最小功能
*     --enable-w32threads       加入多线程支持，会大大提升处理效率
*     --extra-cflags=-O0        额外的编译参数，如 -O0 表示不优化， -I/usr/local/include 表示头文件目录
*     --extra-ldflags=xxx       额外的链接参数，如 -L/usr/local/lib 表示链接目录
*  调试
*    av_log_set_callback -- 设置日志的回调函数
*    av_log_set_level -- 设置日志等级(如 AV_LOG_DEBUG )
*    av_dump_format -- 导出 FormatCtx 的信息
* 
*  编解码库（ffmpeg 只是一个框架，不同的编解码库在不同的网站？http://ffmpeg.zeranoe.com/builds/），
*    可通过 --extra-cflags 来指定头文件的文件夹?
*    libavfilter -- 替代libswscale，用于做图像的pixel format转换，基于DirectShow 机制？
*      AVFilterGraph -- 几乎完全等同与directShow中的fitlerGraph
*      AVFilter -- 代表一个Filter
*      AVFilterPad -- 代表一个filter的输入或输出口，等同于DShow中的Pin
*      AVFilterLink -- 代表两个连接的fitler之间的粘合物
*    libx264 
*    libfaac 
*    libmp3lame -- linux 下支持mp3
*    libogg 
*    libspeex 
*    libswscale -- 
*    libvorbis 
*    libtheora  
* 
*
*  基本概念
*    播放流程: video.avi(Container) -> 打开得到 Video_stream -> 读取Packet -> 解析到 Frame -> 显示Frame
*    Container -- 在音视频中的容器，一般指的是一种特定的文件格式(如 AVI/QT )，里面指明了所包含的音视频，字幕等相关信息
*    Stream -- 可理解为单纯的音频数据或者视频数据等
*    Frame -- Stream中的一个数据单元(AVFrame?)
*    Packet -- Stream中的Raw数据,包含了可以被解码成方便我们最后在应用程序中操作的帧的原始数据(注意Packet和Frame不一定相等)
*    CODEC -- 编解码器(Code 和 Decode)，如 Divx和 MP3
*     
*
*  源码分析
*    AVPicture
*     +-AVFrame
*    AVPacket -- av_read_frame读取一个包并且把它保存到AVPacket::data指针指向的内存中(ffmpeg分配?)
*    
*    AVFormatContext -- 通过其 ->streams->codec->codec_type 判断类型，如 CODEC_TYPE_VIDEO
*    AVCodecContext -- 编解码器上下文，包含了流中所使用的关于编解码器的所有信息，
*      time_base -- 采用分数(den/num)保存了帧率的信息,如 25fps(25/1) NTSC的 29.97fps(den/num= (framenum / 17982) / (framenum % 17982)) ?
*    AVCodec -- 通过 avcodec_find_decoder 查找， avcodec_open2 打开
*    SwsContext -- ?
* 
*    Demuxer -- 媒体格式解码器
*    struct AVCodec -- codec 的数据结构，全局变量 *first_avcodec, avcodec_register_all 方法注册全部 codec
*    解码播放：Media -> ByteIOContext -> Demuxer -> Decode -> YUV -> Display
*    格式转换：Media -> ByteIOContext -> Demuxer -> Decode -> YUV -> Encode -> Muxer -> ByteIOContex -> Media
*    格式转换，如 文件类型从 .flv -> mp4
*    编码类型，如 codec 从 mpeg4 -> H264
*    媒体录制：YUV -> Encode -> Muxer -> ByteIOContex -> Media
*
* 编码流程
*   av_register_all();  //注册了所有可用的文件格式和编解码器的库(可以通过 XXXPolicy.xml 文件来指定支持的格式 ?)
*   AVCodec *pCodecH264 = avcodec_find_encoder(CODEC_ID_H264); //查找h264编码器
*   AVCodecContext *pContext = avcodec_alloc_context3(pCodecH264); //获取初始的编码器上下文，后面设置编码器的参数
*   pContext->codec_id = pCodecH264->id; //CODEC_ID_H264
*   pContext->codec_type = AVMEDIA_TYPE_VIDEO;
*   pContext->bit_rate = 3000000; pContext->width=nWidth; pContext->height=nHeight;
*   pContext->time_base.num = 1; pContext->time_base.den = 25; //每秒25帧, 或 den=(int)(UNITS / pVih->AvgTimePerFrame);
*   pContext->gop_size = 12; //每12帧放一个GOP
*   pContext->thread_count = 2; //多线程个数
*   pContext->thread_type = FF_THREAD_SLICE;  //设置使用多个线程来同时编解码一帧
*   pContext->pix_fmt = PIX_FMT_YUV420P;  //设置颜色空间格式?
*   //pContext->flags |= CODEC_FLAG_GLOBAL_HEADER; //mp4,mov,3gp 等设置这个参数?
*   avcodec_open2(pContext, pCodecH264, NULL); //打开编码库，使用完毕后需要 avcodec_close
*   AVFrame *m_pRGBFrame = avcodec_alloc_frame();  //RGB帧数据
*   numBytes=avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height); //计算需要的空间
*   rgb_buff = av_malloc(numBytes*sizeof(uint8_t));  //分配内存, av_malloc 可以保证内存对齐
*   avpicture_fill((AVPicture*)m_pRGBFrame, (uint8_t*)rgb_buff, PIX_FMT_RGB24, nWidth, nHeight); //填充原始的RGB数据
*  
*   //ffmpeg中大多数的视频格式好像只支持YUV格式的视频帧(AVFrame)，必须先把RGB的视频帧转成YUV视频帧(sws_scale),
*   //  m_pYUVFrame 中 data[0]是亮度,data[1]是色度,data[2] 也是色度?
*   //  但是播放时又需要转换为 RGB 格式 ?
*   SwsContext * scxt = sws_getContext(pContext->width, pContext->height, PIX_FMT_RGB24, 
*      pContext->width, pContext->height, PIX_FMT_YUV420P, SWS_FAST_BILINEAR, NULL, NULL, NULL);  //旧的方式是 img_convert ?
*   sws_scale 有问题?( ctx,pFrameRGB->data,pFrameRGB->linesize,0,pCodecCtx->height,pFrame->data,pFrame->linesize);  
*   int iOutSize = avcodec_encode_video(m_pContext, pBufferOut, lOutBufferSize, m_pYuvFrame);//编码
*     //或 avcodec_encode_video2(c, &avpkt, m_pYUVFrame, &got_packet_ptr); 
*     //avcodec_decode_video/avcodec_decode_video2 把包转换为帧，得到完整的一帧时，会设置了帧结束标志frameFinished
*   sws_freeContext(scxt)
*   av_free_packet(&packet);
*    
*
* libavFilter 使用流程
*   AVFilterGraph *graph = avfilter_graph_alloc();
*   AVFilterContext *filt_src = NULL;
*   avfilter_graph_create_filter(&filt_src, &input_filter, "src",NULL, is, graph); //input_filter必须由调用者实现
*   AVFilterContext *filt_out = NULL;
*   avfilter_graph_create_filter(&filt_out, avfilter_get_by_name("buffersink"), "out", NULL, pix_fmts, graph);
*   avfilter_link(filt_src, 0, filt_out, 0); //连接source和sink
*   avfilter_graph_config(graph, NULL); //
*   实现input_filter
*     static AVFilter input_filter = { .name="ffplay_input", .priv_size=sizeof(FilterPriv), .init=input_init, xxx};
*     
*   
*********************************************************************************************/

// 填充RGB然后翻转，好转换为YUV  
#if 0
avpicture_fill((AVPicture*)m_pRGBFrame, (uint8_t*)rgb_buff, PIX_FMT_RGB24, nWidth, nHeight);  
m_pRGBFrame->data[0]  += m_pRGBFrame->linesize[0] * (nHeight - 1);  
m_pRGBFrame->linesize[0] *= -1;                     
m_pRGBFrame->data[1]  += m_pRGBFrame->linesize[1] * (nHeight / 2 - 1);  
m_pRGBFrame->linesize[1] *= -1;  
m_pRGBFrame->data[2]  += m_pRGBFrame->linesize[2] * (nHeight / 2 - 1);  
m_pRGBFrame->linesize[2] *= -1; 
sws_scale(scxt,m_pRGBFrame->data,m_pRGBFrame->linesize,0,c->height,m_pYUVFrame->data,m_pYUVFrame->linesize);
#endif 