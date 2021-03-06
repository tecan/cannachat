/*
  Copyright (C) 2008-2016 The Communi Project

  You may use this file under the terms of BSD license as follows:

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the copyright holder nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef IRCCOMMANDQUEUE_P_H
#define IRCCOMMANDQUEUE_P_H

#include "irccommandqueue.h"
#include "ircfilter.h"
#include <QPointer>
#include <QQueue>
#include <QTimer>

IRC_BEGIN_NAMESPACE

class IrcCommandQueuePrivate : public QObject,  public IrcCommandFilter
{
    Q_OBJECT
    Q_INTERFACES(IrcCommandFilter)
    Q_DECLARE_PUBLIC(IrcCommandQueue)

public:
    IrcCommandQueuePrivate();

    bool commandFilter(IrcCommand* cmd);

    void _irc_updateTimer();
    void _irc_sendBatch(bool force = false);

    IrcCommandQueue* q_ptr;
    IrcConnection* connection;
    QTimer timer;
    int batch;
    int interval;
    QQueue<QPointer<IrcCommand> > commands;
};

IRC_END_NAMESPACE

#endif // IRCCOMMANDQUEUE_P_H
