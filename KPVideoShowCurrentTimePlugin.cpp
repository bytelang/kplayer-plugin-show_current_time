//
// Created by karaler on 2019/11/21.
// Author: karaler
// Description: 
// Others: 
// Copyright (c) 2019 Bytelang . All rights reserved.
//

#include "KPVideoShowCurrentTimePlugin.h"

KPVideoShowCurrentTimePlugin::KPVideoShowCurrentTimePlugin(const std::string &identify_name, const std::string &filter_name, const KPFilterType &filter_type, PluginParamsObject plugin_params_object) : KPPluginAdapter(identify_name, filter_name, filter_type, plugin_params_object) {
    std::string font_size  = "17";
    std::string font_color = "white";
    std::string x          = "10";
    std::string y          = "10";

    if (plugin_params_object.params.find("font_size") != plugin_params_object.params.end()) {
        font_size = plugin_params_object.params["font_size"];
    }
    if (plugin_params_object.params.find("font_color") != plugin_params_object.params.end()) {
        font_color = plugin_params_object.params["font_color"];
    }
    if (plugin_params_object.params.find("x") != plugin_params_object.params.end()) {
        x = plugin_params_object.params["x"];
    }
    if (plugin_params_object.params.find("y") != plugin_params_object.params.end()) {
        y = plugin_params_object.params["y"];
    }

    // 赋值described
    std::stringstream filter_desc_stream;
    filter_desc_stream << "x=" << x << ":y=" << y << ":fontfile=res/font.ttf:fontsize=" << font_size << ":fontcolor=" << font_color << ":text='" << "[无]" << "'";
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
        std::this_thread::sleep_for(std::chrono::microseconds(1000));

        time_t now_time;
        time(& now_time);
        char str_time[64];
        strftime(str_time, sizeof(str_time), "%Y-%m-%d %H:%M:%S", localtime(& now_time));

        SetPluginValue(std::map<std::string, std::string>{{"text", str_time}});
    } while (!stop);
}

void KPVideoShowCurrentTimePlugin::KillTask() {
    stop = true;
}

void KPVideoShowCurrentTimePlugin::InitTask() {
    stop = false;
}

// slot
KPLAYER_PLUGIN_FUNC(KPVideoShowCurrentTimePlugin) {
    return new KPVideoShowCurrentTimePlugin("kplayer", "video_plugin_show_current_time", KP_FILTER_TYPE_VIDEO, std::move(plugin_params));
}
