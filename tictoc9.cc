

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>



using namespace omnetpp;

#include "tictoc13_m.h"

/**
 * In the previous model we just created another packet if we needed to
 * retransmit. This is OK because the packet didn't contain much, but
 * in real life it's usually more practical to keep a copy of the original
 * packet so that we can re-send it without the need to build it again.
 */
class Txc4 : public cSimpleModule
{
  private:
    int counter;
    int prio;
    simtime_t time = 0;


  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Txc4);

void Txc4::initialize()
{
    // Initialize the counter with the "limit" module parameter, declared
    // in the NED file (tictoc4.ned).
    counter = par("limit");
    prio = par("priority");
    WATCH(time);

    // we no longer depend on the name of the module to decide
    // whether to send an initial message

        EV << "Sending initial message\n";

        TicTocMsg13 *msgt = new TicTocMsg13("tictocmsg");
        msgt->setPriority(prio);
        msgt->setSource(0);
        send(msgt, "out");

}

void Txc4::handleMessage(cMessage *msg)
{
    TicTocMsg13 *ttmsg = check_and_cast<TicTocMsg13 *>(msg);

    if (counter == 0) {
        time = simTime();

        EV << getName() << "'s counter reached zero at" << simTime().str() <<", deleting message\n";

        bubble("finished");
        delete ttmsg;
    }
    else {
        counter --;
        EV << getName() << "'s counter is " << counter << ", sending back message\n";
        ttmsg->setSource(0);
        send(ttmsg, "out");
    }
}

/**
 * Sends back an acknowledgement -- or not.
 */
class Toc9 : public cSimpleModule
{
  protected:
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Toc9);

void Toc9::handleMessage(cMessage *msg)
{


    TicTocMsg13 *ttmsg = check_and_cast<TicTocMsg13 *>(msg);


        EV << ttmsg << " received, sending back an acknowledgement.\n";

        ttmsg->setSource(1);

        send(ttmsg, "out");

}

class Router : public cSimpleModule
{
  protected:
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Router);


void Router::handleMessage(cMessage *msg)
{


    TicTocMsg13 *ttmsg = check_and_cast<TicTocMsg13 *>(msg);

    int p = ttmsg->getPriority();
    int s = ttmsg->getSource();

    if (p==0) {   //normal
        EV << "normal " << ttmsg << endl;
        if(s==0){
            send(ttmsg,"outN");
        }else{
            send(ttmsg,"homeN");
        }

    }
    else { //prio
        EV << "priority " << ttmsg << endl;
                if(s==0){
                    send(ttmsg,"outP");
                }else{
                    send(ttmsg,"homeP");
                }
    }
}



