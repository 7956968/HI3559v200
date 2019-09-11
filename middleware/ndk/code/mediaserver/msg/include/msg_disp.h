/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2019. All rights reserved.
 *
 * @file    msg_disp.h
 * @brief   msg disp header
 * @author  HiMobileCam NDK develop team
 * @date  2019-3-26
 */
#ifndef __MSG_DISP_H__
#define __MSG_DISP_H__

typedef enum tagMSG_DISP_CMD_E {
    MSG_CMD_DISP_INIT = 0,
    MSG_CMD_DISP_DEINIT,

    MSG_CMD_DISP_START,
    MSG_CMD_DISP_STOP,

    MSG_CMD_DISP_SET_WINDOW_ATTR,
    MSG_CMD_DISP_GET_WINDOW_ATTR,

    MSG_CMD_DISP_SET_WINDOW_ATTREX,
    MSG_CMD_DISP_GET_WINDOW_ATTREX,

    MSG_CMD_DISP_START_WINDOW,
    MSG_CMD_DISP_STOP_WINDOW,
    MSG_CMD_DISP_CLEAR_WINDOW,

    MSG_CMD_DISP_BIND_VPROC,
    MSG_CMD_DISP_UNBIND_VPROC,

    MSG_CMD_DISP_BIND_VCAP,
    MSG_CMD_DISP_UNBIND_VCAP,

    MSG_CMD_DISP_SET_ATTREX,
    MSG_CMD_DISP_GET_ATTREX,

    MSG_CMD_DISP_SET_WINDOW_OSD_ATTR,
    MSG_CMD_DISP_GET_WINDOW_OSD_ATTR,

    MSG_CMD_DISP_START_WINDOW_OSD,
    MSG_CMD_DISP_STOP_WINDOW_OSD,

} MSG_DISP_CMD_E;

#endif /* __MSG_DISP_H__ */