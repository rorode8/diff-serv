//
//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2003-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//


simple Txc4
{
    parameters:
        @display("i=device/laptop");
        int priority = default(0);
        int limit = default(20);
    gates:
        input in;
        output out;
}

simple Router
{
    parameters:
        @display("i=abstract/router");
    gates:
        input inP;
        input inN;
        input inS;

        output homeP;
        output homeN;
        output outP;
        output outN;
}

simple Toc9
{
    parameters:
        @display("i=device/pc");

    gates:
        input inP;
        input inN;
        output out;
}

//
// Same as Tictoc8.
//
network Tictoc9
{
    @display("bgb=494,289");
    submodules:
        ticP: Txc4 {
            parameters:
                @display("i=,cyan;p=427,36");
                priority = 1;
        }
        ticN: Txc4 {
            parameters:
                @display("i=,cyan;p=388,236");
                priority = 0;
        }
        router: Router {
            parameters:
                @display("i=,cyan;p=244,135");

        }
        toc: Toc9 {
            parameters:
                @display("i=,gold;p=36,170");
        }
    connections:
        ticP.out --> {  delay = 20ms; } --> router.inP;
        ticN.out --> {  delay = 100ms; } --> router.inN;
        router.homeN --> {  delay = 100ms; } --> ticN.in;
        router.homeP --> {  delay = 20ms; } --> ticP.in;
        router.outN --> {  delay = 100ms; } --> toc.inN;
        router.outP --> {  delay = 20ms; } --> toc.inP;
        toc.out --> {  delay = 100ms; } --> router.inS;
}
