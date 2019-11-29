//
// Created by karaler on 2019/11/21.
// Author: karaler
// Description: 
// Others: 
// Copyright (c) 2019 Bytelang . All rights reserved.
//

#include "KPVideoShowCurrentTimePlugin.h"

KPVideoShowCurrentTimePlugin::KPVideoShowCurrentTimePlugin(const std::string &identify_name, PluginParams params) : KPPluginAdapter(identify_name) {
    // 赋值信息
    filter_type = KP_FILTER_TYPE_VIDEO;
    filter_name = "video_plugin_show_time";

    std::string font_size  = "17";
    std::string font_color = "white";
    if (params.find("font_size") != params.end()) {
        font_size = params["font_size"];
    }
    if (params.find("font_color") != params.end()) {
        font_color = params["font_color"];
    }

    // 赋值described
    std::stringstream filter_desc_stream;
    filter_desc_stream << "x=10:y=10:fontfile=Res/font.ttf:fontsize=" << font_size << ":fontcolor=" << font_color << ":text='" << "[无]" << "'";
    filter_desc = filter_desc_stream.str();

    // 查找过滤器
    filter = avfilter_get_by_name("drawtext");
    if (filter == nullptr)
        throw KPFilterException("未知过滤器; filter name: drawtext");
}

/**
 * 秒级定时器，一秒延迟之后更新plugin text内容
 * 在plugin可用时会使用单独的子线程开启task，需要主要判断filter_context当前是否可用
 * 在对plugin reload时 filter_context会短暂不可用
 *
 */
void KPVideoShowCurrentTimePlugin::Task() {
    do {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        AVDictionary *dict = nullptr;

        time_t now_time;
        time(& now_time);
        char str_time[64];
        strftime(str_time, sizeof(str_time), "%Y-%m-%d %H:%M:%S", localtime(& now_time));
        av_dict_set(& dict, "text", str_time, 0);

        void *priv = GetFilterPriv();
        if (!priv) {
            logger->error("无法对plugin option进行设置; error: {}", "priv为空");
            break;
        }

        int ret = av_opt_set_dict(priv, & dict);
        if (ret < 0) {
            logger->error("无法对plugin option进行设置;");
            break;
        }
    } while (!stop);
}

void KPVideoShowCurrentTimePlugin::KillTask() {
    stop = true;
}

void KPVideoShowCurrentTimePlugin::InitTask() {
    stop = false;
}

// slot
__attribute__ ((visibility ("default"))) KPVideoShowCurrentTimePlugin *CreateInstance(PluginParams params) {
    return new KPVideoShowCurrentTimePlugin("kplayer", params);
}
