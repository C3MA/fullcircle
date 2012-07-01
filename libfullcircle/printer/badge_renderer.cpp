#include "badge_renderer.hpp"
#include <sstream>
#include <boost/filesystem/fstream.hpp>
#include <boost/regex.hpp>
// see also http://www.johndcook.com/cpp_regex.html for an introduction
// to C++11 regexes.

using namespace fullcircle;

std::string BadgeRenderer::replace_number(const std::string& input,
    const std::string& number) {
   static const boost::regex e("@@number@@", 
       boost::regex::perl|boost::regex::icase);
   return regex_replace(input, e, number, boost::match_default);
}

std::string BadgeRenderer::replace_color(const std::string& input,
    const std::string& color) {
   static const boost::regex e("@@COLOR@@", 
       boost::regex::perl|boost::regex::icase);
   return regex_replace(input, e, color, boost::match_default);
}

// see http://www.pvgenerator.de/index.php/off-topic/bondrucker-tm-88-mit-php-die-zweite/71
// for replacement table
std::string BadgeRenderer::replace_umlaute(const std::string& input) {
   static const boost::regex Auml("Ä", boost::regex::perl);
   static const boost::regex auml("ä", boost::regex::perl);
   static const boost::regex Uuml("Ü", boost::regex::perl);
   static const boost::regex uuml("ü", boost::regex::perl);
   static const boost::regex Ouml("Ö", boost::regex::perl);
   static const boost::regex ouml("ö", boost::regex::perl);
   static const boost::regex suml("ß", boost::regex::perl);
   return 
     regex_replace(
       regex_replace(
         regex_replace(
           regex_replace(
             regex_replace(
               regex_replace(
                 regex_replace(
                   input, Auml, std::string("\x8e"))
               , auml, std::string("\x84"))
             , Uuml, std::string("\x9a"))
           , uuml, std::string("\x81"))
         , Ouml, std::string("\x99"))
       , ouml, std::string("\x94"))
     , suml, std::string("\xe1"));
}



std::string BadgeRenderer::render(
    const std::string& color,
    const std::string& number
) {
  bfs::ifstream ifs(_templatefile);
  std::ostringstream oss;
  // prepend setup code:
  // ESC @: Initializes printer
  // ESC !: Select font A 
  // ESC t: Select code table 0 (standard)
   oss << "\x1b@"; //\x1b!\x00\x1bt\x00";
  std::string line;
  if (ifs.is_open()) {
    while (ifs.good()) {
      getline(ifs, line);
      oss << line << std::endl;
    }
  } else {
    std::ostringstream oss2;
    oss << "Cannot read from file " << _templatefile;
    throw RenderException(oss.str());
  }
  return replace_umlaute(
          replace_color(
            replace_number(
              oss.str(), number
            ), color
          )
        );
}
