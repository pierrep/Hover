import qbs
import qbs.Process
import qbs.File
import qbs.FileInfo
import qbs.TextFile
import "../../../libs/openFrameworksCompiled/project/qtcreator/ofApp.qbs" as ofApp

Project{
    property string of_root: "../../.."

    ofApp {
        name: { return FileInfo.baseName(path) }

        files: [
            "src/dsp/crush/crush.cpp",
            "src/dsp/crush/crush.h",
            "src/dsp/foldbackDistortion/foldBackDistortion.cpp",
            "src/dsp/foldbackDistortion/foldBackDistortion.h",
            "src/dsp/phaser/phasor.cpp",
            "src/dsp/phaser/phasor.h",
            "src/dsp/reverb/allpass.cpp",
            "src/dsp/reverb/allpass.hpp",
            "src/dsp/reverb/comb.cpp",
            "src/dsp/reverb/comb.hpp",
            "src/dsp/reverb/denormals.h",
            "src/dsp/reverb/revmodel.cpp",
            "src/dsp/reverb/revmodel.hpp",
            "src/dsp/reverb/tuning.h",
            'src/DeclickedFLoat.h',
            'src/InteractivePad.h',
            'src/PadEvent.cpp',
            'src/PadEvent.h',
            'src/SoundEngine.cpp',
            'src/SoundEngine.h',
            'src/main.cpp',
            'src/ofApp.cpp',
            'src/ofApp.h',
            'src/synths/CompressorSynth.cpp',
            'src/synths/ControlSnapToScaleTestSynth.cpp',
            'src/synths/FMDroneSynth.cpp',
            'src/synths/FilteredNoiseSynth.cpp',
            'src/synths/ReverbTestSynth.cpp',
            'src/synths/SampleWithReverb.cpp',
            'src/synths/Sampler.cpp',
            'src/synths/StepSequencer.cpp',
            'src/synths/StereoDelaySynth.cpp',
            'src/synths/SynaesthesiaSynth.cpp',
            'src/synths/XYSpeed.cpp',
        ]

        of.addons: [
            'ofxLeapMotion',
            'ofxMidi',
            'ofxMSAInteractiveObject',
            'ofxOpenCv',
            'ofxOsc',
            'ofxTonic',
        ]

        // additional flags for the project. the of module sets some
        // flags by default to add the core libraries, search paths...
        // this flags can be augmented through the following properties:
        of.pkgConfigs: []       // list of additional system pkgs to include
        of.includePaths: []     // include search paths
        of.cFlags: []           // flags passed to the c compiler
        of.cxxFlags: []         // flags passed to the c++ compiler
        of.linkerFlags: []      // flags passed to the linker
        of.defines: []          // defines are passed as -D to the compiler
                                // and can be checked with #ifdef or #if in the code

        // other flags can be set through the cpp module: http://doc.qt.io/qbs/cpp-module.html
        // eg: this will enable ccache when compiling
        //
        // cpp.compilerWrapper: 'ccache'

        Depends{
            name: "cpp"
        }

        // common rules that parse the include search paths, core libraries...
        Depends{
            name: "of"
        }

        // dependency with the OF library
        Depends{
            name: "openFrameworks"
        }
    }

    references: [FileInfo.joinPaths(of_root, "/libs/openFrameworksCompiled/project/qtcreator/openFrameworks.qbs")]
}
