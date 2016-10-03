/**
 * @brief Offline frontend (using libsndfile) to test Sushi host and plugins
 */

#include <vector>
#include <fstream>
#include <iostream>
#include <cstdlib>

#include "logging.h"
#include "options.h"
#include "audio_frontends/offline_frontend.h"


int main(int argc, char* argv[])
{
    ////////////////////////////////////////////////////////////////////////////////
    // Command Line arguments parsing
    ////////////////////////////////////////////////////////////////////////////////

    // option_parser accepts arguments excluding program name,
    // so skip it if it is present
    if (argc > 0)
    {
        argc--;
        argv++;
    }

    option::Stats cl_stats(usage, argc, argv);
    std::vector<option::Option> cl_options(cl_stats.options_max);
    std::vector<option::Option> cl_buffer(cl_stats.buffer_max);
    option::Parser cl_parser(usage, argc, argv, &cl_options[0], &cl_buffer[0]);

    if (cl_parser.error())
    {
        return 1;
    }
    if ( (cl_parser.nonOptionsCount() != 1) || (cl_options[OPT_IDX_HELP]) )
    {
        option::printUsage(fwrite, stdout, usage);
        return 0;
    }

    std::string input_filename = std::string(cl_parser.nonOption(0));
    // By default, prepend sushiproc_ to filename
    std::string output_filename = std::string(input_filename);
    output_filename.append(".proc.wav");

    std::string log_level = std::string(SUSHI_LOG_LEVEL_DEFAULT);
    std::string log_filename = std::string(SUSHI_LOG_FILENAME_DEFAULT);

    for (int i=0; i<cl_parser.optionsCount(); i++)
    {
        option::Option& opt = cl_buffer[i];
        switch(opt.index())
        {
        case OPT_IDX_HELP:
        case OPT_IDX_UNKNOWN:
            // should be handled before arriving here
            assert(false);
            break;

        case OPT_IDX_LOG_LEVEL:
            log_level.assign(opt.arg);
            break;

        case OPT_IDX_LOG_FILE:
            log_filename.assign(opt.arg);
            break;

        case OPT_IDX_OUTPUT_FILE:
            output_filename.assign(opt.arg);
            break;

        default:
            SushiArg::print_error("Unhandled option '", opt, "' \n");
            break;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    // Logger configuration
    ////////////////////////////////////////////////////////////////////////////////
    auto ret_code = MIND_LOG_SET_PARAMS(log_filename, "Logger", log_level);
    if (ret_code != MIND_LOG_ERROR_CODE_OK)
    {
        fprintf(stderr, "%s, using default.\n", MIND_LOG_GET_ERROR_MESSAGE(ret_code).c_str());
    }

    MIND_GET_LOGGER;

    ////////////////////////////////////////////////////////////////////////////////
    // Main body
    ////////////////////////////////////////////////////////////////////////////////
    sushi::audio_frontend::OfflineFrontendConfiguration offline_config(48000,
                                                                       2,
                                                                       input_filename,
                                                                       output_filename);
    sushi::audio_frontend::OfflineFrontend frontend;
    auto fe_ret_code = frontend.init(&offline_config);
    if (fe_ret_code != sushi::audio_frontend::AudioFrontendInitStatus::OK)
    {
        fprintf(stderr, "Error initializing frontend, check logs for details.\n");
        exit(1);
    }

    frontend.run();

    return 0;
}

