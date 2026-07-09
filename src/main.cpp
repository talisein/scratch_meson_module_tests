import std;
import Gio;
import fmt_using_library;
import cpr;

int main()
{
  auto home = peel::Gio::File::create_for_path (std::getenv("HOME"));
  auto enumerator = home->enumerate_children ("standard::name",
                                              peel::Gio::File::QueryInfoFlags::NONE,
                                              nullptr, nullptr);
  if (!enumerator) {
    std::println ("enumerate_children failed");
    return 1;
  }

  std::println("First three files in home with *amazing* formatting:");
  for (int i = 0; i < 3; ++i) {
    auto info = enumerator->next_file (nullptr, nullptr);
    std::println ("{}", fmt_using_library::make_amazing(info->get_name()));
  }

  auto response = cpr::Get(cpr::Url{"https://mesonbuild.com/Release-notes.html"});
  std::println("cpr worked! HTTP code {}", response.status_code);

  return 0;
}
