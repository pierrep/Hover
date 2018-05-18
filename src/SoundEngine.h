#pragma once

#include "Tonic.h"

using namespace Tonic;

class SoundEngine : public Tonic::Synth
{
    public:
        SoundEngine();
        virtual ~SoundEngine();
        void setup();

        inline Synth& getSynth(int index) {
            return (*(synths.at(index)));
        }

        inline int getNumSynths() {
            return synths.size();
        }

        inline ControlParameter& getFX1Send(int index) {
            //return this->getParameters().at(index);
//            vector<ControlParameter> params = this->getParameters();
//            cout << "----------------------------------------" << endl;
//            for(int i = 0; i < params.size();i++){
//                cout << "param name= " + params[i].getName() << "value=" << params[i].getValue() << endl;
//            }
            return FX1Send.at(index);
        }

        int createSynth(string filename);

        void recalculateSynths();

    protected:
        vector<Tonic::Synth *> synths;
        vector<Tonic::ControlParameter>  FX1Send;
        Tonic::Mixer    mixer;
        Tonic::Reverb   reverb;

    private:

};

