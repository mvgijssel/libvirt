/*
 * qemu_agent.h: interaction with QEMU guest agent
 *
 * Copyright (C) 2006-2012 Red Hat, Inc.
 * Copyright (C) 2006 Daniel P. Berrange
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "internal.h"
#include "domain_conf.h"

typedef struct _qemuAgent qemuAgent;
typedef qemuAgent *qemuAgentPtr;

typedef struct _qemuAgentCallbacks qemuAgentCallbacks;
typedef qemuAgentCallbacks *qemuAgentCallbacksPtr;
struct _qemuAgentCallbacks {
    void (*destroy)(qemuAgentPtr mon,
                    virDomainObjPtr vm);
    void (*eofNotify)(qemuAgentPtr mon,
                      virDomainObjPtr vm);
    void (*errorNotify)(qemuAgentPtr mon,
                        virDomainObjPtr vm);
};


qemuAgentPtr qemuAgentOpen(virDomainObjPtr vm,
                           const virDomainChrSourceDef *config,
                           qemuAgentCallbacksPtr cb);

void qemuAgentClose(qemuAgentPtr mon);

void qemuAgentNotifyClose(qemuAgentPtr mon);

typedef enum {
    QEMU_AGENT_EVENT_NONE = 0,
    QEMU_AGENT_EVENT_SHUTDOWN,
    QEMU_AGENT_EVENT_SUSPEND,
    QEMU_AGENT_EVENT_RESET,
} qemuAgentEvent;

void qemuAgentNotifyEvent(qemuAgentPtr mon,
                          qemuAgentEvent event);

typedef enum {
    QEMU_AGENT_SHUTDOWN_POWERDOWN,
    QEMU_AGENT_SHUTDOWN_REBOOT,
    QEMU_AGENT_SHUTDOWN_HALT,

    QEMU_AGENT_SHUTDOWN_LAST,
} qemuAgentShutdownMode;

typedef struct _qemuAgentDiskInfo qemuAgentDiskInfo;
typedef qemuAgentDiskInfo *qemuAgentDiskInfoPtr;
struct _qemuAgentDiskInfo {
    char *serial;
    virPCIDeviceAddress pci_controller;
    char *bus_type;
    unsigned int bus;
    unsigned int target;
    unsigned int unit;
    char *devnode;
};

typedef struct _qemuAgentFSInfo qemuAgentFSInfo;
typedef qemuAgentFSInfo *qemuAgentFSInfoPtr;
struct _qemuAgentFSInfo {
    char *mountpoint; /* path to mount point */
    char *name;       /* device name in the guest (e.g. "sda1") */
    char *fstype;     /* filesystem type */
    long long total_bytes;
    long long used_bytes;
    size_t ndisks;
    qemuAgentDiskInfoPtr *disks;
};
void qemuAgentFSInfoFree(qemuAgentFSInfoPtr info);

int qemuAgentShutdown(qemuAgentPtr mon,
                      qemuAgentShutdownMode mode);

int qemuAgentFSFreeze(qemuAgentPtr mon,
                      const char **mountpoints, unsigned int nmountpoints);
int qemuAgentFSThaw(qemuAgentPtr mon);
int qemuAgentGetFSInfo(qemuAgentPtr mon, qemuAgentFSInfoPtr **info);

int qemuAgentSuspend(qemuAgentPtr mon,
                     unsigned int target);

int qemuAgentArbitraryCommand(qemuAgentPtr mon,
                              const char *cmd,
                              char **result,
                              int timeout);
int qemuAgentFSTrim(qemuAgentPtr mon,
                    unsigned long long minimum);


typedef struct _qemuAgentCPUInfo qemuAgentCPUInfo;
typedef qemuAgentCPUInfo *qemuAgentCPUInfoPtr;
struct _qemuAgentCPUInfo {
    unsigned int id;    /* logical cpu ID */
    bool online;        /* true if the CPU is activated */
    bool offlinable;    /* true if the CPU can be offlined */

    bool modified; /* set to true if the vcpu state needs to be changed */
};

int qemuAgentGetVCPUs(qemuAgentPtr mon, qemuAgentCPUInfoPtr *info);
int qemuAgentSetVCPUs(qemuAgentPtr mon, qemuAgentCPUInfoPtr cpus, size_t ncpus);
int qemuAgentUpdateCPUInfo(unsigned int nvcpus,
                           qemuAgentCPUInfoPtr cpuinfo,
                           int ncpuinfo);

int
qemuAgentGetHostname(qemuAgentPtr mon,
                     char **hostname);

int qemuAgentGetTime(qemuAgentPtr mon,
                     long long *seconds,
                     unsigned int *nseconds);
int qemuAgentSetTime(qemuAgentPtr mon,
                     long long seconds,
                     unsigned int nseconds,
                     bool sync);

int qemuAgentGetInterfaces(qemuAgentPtr mon,
                           virDomainInterfacePtr **ifaces);

int qemuAgentSetUserPassword(qemuAgentPtr mon,
                             const char *user,
                             const char *password,
                             bool crypted);

int qemuAgentGetUsers(qemuAgentPtr mon,
                      virTypedParameterPtr *params,
                      int *nparams,
                      int *maxparams);

int qemuAgentGetOSInfo(qemuAgentPtr mon,
                       virTypedParameterPtr *params,
                       int *nparams,
                       int *maxparams);

int qemuAgentGetTimezone(qemuAgentPtr mon,
                         virTypedParameterPtr *params,
                         int *nparams,
                         int *maxparams);

void qemuAgentSetResponseTimeout(qemuAgentPtr mon,
                                 int timeout);
