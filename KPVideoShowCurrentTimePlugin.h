//
// Created by karaler on 2019/11/21.
// Author: karaler
// Description: 
// Others: 
// Copyright (c) 2019 Bytelang . All rights reserved.
//

#ifndef KPLAYER_KPVIDEOSHOWCURRENTTIMEPLUGIN_H
#define KPLAYER_KPVIDEOSHOWCURRENTTIMEPLUGIN_H

#include "adapter/KPPluginAdapter.h"
#include <utility>

class KPVideoShowCurrentTimePlugin : public KPPluginAdapter {
protected:
    void Task() override;
    bool stop = false;
public:
    explicit KPVideoShowCurrentTimePlugin(const std::string &identify_name, const std::string &filter_name, const KPFilterType &filter_type, PluginParamsObject plugin_params_object);
    ~KPVideoShowCurrentTimePlugin() override = default;
    void KillTask() override;
    void InitTask() override;
};

// slot
extern "C" {
KPLAYER_PLUGIN_FUNC(KPVideoShowCurrentTimePlugin);
}


#endif //KPLAYER_KPVIDEOSHOWCURRENTTIMEPLUGIN_H
