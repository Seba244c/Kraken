#define KRAKEN_APP
#include "Kraken.h"

class TappyBird : public Kraken::Application {
public:
	explicit TappyBird(const Kraken::ApplicationInfo &applicationInfo)
        : Application(applicationInfo) {
    }
};

Kraken::Application *Kraken::CreateApplication(AppCommandlineArguments cmdlineArguments) {
    ApplicationInfo info;
    info.Name = "TappyBird";
    info.Author = "ssnoer";
    info.Version = "1.0.0";
    
    return new TappyBird(info);
}
