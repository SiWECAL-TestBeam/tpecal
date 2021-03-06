//
//  ChipBuffer.h
//  cobana_project
//
//  Created by Roman Poeschl on 29/07/15.
//
//

#ifndef __cobana_project__ChipBuffer__
#define __cobana_project__ChipBuffer__

#include <stdio.h>
#include <vector>
#include <TFile.h>
#include "Channel.h"
#include "TH1F.h"

class ChipBuffer {
    
public:
    ChipBuffer(unsigned);
    ~ChipBuffer();
    void init(unsigned);
    //A method to acquire the values of the individual channels
    bool setChannelVals(Int_t, Int_t, Int_t, Int_t, Int_t, Int_t);
    //Set the number oc channels, a fall back solution in case a proper mapping file is missing
    void setNumberOfASICChannels(unsigned);
    //Number of channels served by an individual buffer
    unsigned getNumberOfChannels();
    //A method to access relevant values of indivudual channels I.E. mean and rms, for low/high and hit bit=0,1 or both (option 0,1,-1)
    double getChannelMean(unsigned, std::string, Int_t);
    double getChannelRMS(unsigned, std::string, int);
    std::vector<double> getChannelMeanVec(unsigned, std::string);
    std::vector<double> getChannelRMSVec(unsigned, std::string);

    //A method to return the number of entries for the channel (only for high has to be extended for low)
    unsigned getChannelEntries(unsigned);
    //A method to return the number of triggers for the channel
    std::vector<unsigned> getChannelTriggersVec(unsigned);

    //Get the total number of triggers in this buffer
    unsigned getTotalNumberOfTriggers();

    //A method to access relevant Histograms of indivudual channels for low/high gain = 0,1 (channel, gain)
    std::vector<int> getChannelPedHisto(unsigned,unsigned);
    std::vector<int> getChannelChargeHisto(unsigned,unsigned);

    //chip monitoring tools
    void setChipVals(Int_t , Int_t , Int_t , Int_t );
    std::vector<Int_t> getBcidVec();
    std::vector<Int_t> getNhitsRate();



private:
    //the buffer ID (=bufferDepth)
    unsigned _bufferID;
    //the number of channels in each buffer
    unsigned _numChans;
    //create a vector that holds the channels
    std::vector<Channel> _channelVec;
    std::vector<std::vector<int> > _channelPedHighHistoVec;
    std::vector<std::vector<int> > _channelHighHistoVec;
    std::vector<std::vector<int> > _channelPedLowHistoVec;
    std::vector<std::vector<int> > _channelLowHistoVec;

    //a counter thqt holds the acquired channels
    unsigned _channelCount;
    
    //chip buffer monitoring objects
    std::vector<Int_t> _nhitsVec;
    std::vector<Int_t> _chipBcidVec;

};

#endif /* defined(__cobana_project__ChipBuffer__) */
