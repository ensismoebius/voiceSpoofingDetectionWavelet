/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 16 de aug de 2023
 *
 * The experiment 01 consists in... no idea for now
 */

#ifndef SRC_IMAGINEDSPEECHEXPERIMENTS_01_EXPERIMENT01_CPP_
#define SRC_IMAGINEDSPEECHEXPERIMENTS_01_EXPERIMENT01_CPP_

#include <cmath>
#include <future>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "../../lib/linearAlgebra/linearAlgebra.h"
#include "../../lib/matplotlib-cpp/matplotlibcpp.h"
#include "../../lib/paraconsistent/paraconsistent.h"
#include "../../lib/wave/Wav.h"
#include "../../lib/wavelet/Types.h"
#include "../../lib/wavelet/WaveletTransformResults.h"
#include "../../lib/wavelet/waveletOperations.h"

namespace imaginedSpeechExperiments {

/**
* Contains the code for experiment 01.
*/
class Experiment01
{
private:
public:
    /**
    * Initializes the experiment
    */
    static void init() { std::cout << "First experiment" << std::endl; }
    static void perform() { std::cout << "First experiment" << std::endl; }
};
} // namespace imaginedSpeechExperiments

#endif /* SRC_IMAGINEDSPEECHEXPERIMENTS_01_EXPERIMENT01_CPP_ */
