#include <routingkit/contraction_hierarchy.h>
#include <emscripten/val.h>
#include <emscripten/bind.h>

#include <routingkitjs/ch.h>

using namespace emscripten;

namespace RoutingKitJS
{

  auto get_callback(val cb)
  {
    return [cb](const std::string &msg)
    {
      cb.call<void>("call", 0, msg);
    };
  }

  CH CH::build(
      unsigned node_count,
      const std::vector<unsigned> &tail,
      const std::vector<unsigned> &head,
      const std::vector<unsigned> &weight,
      val log_message,
      unsigned max_pop_count)
  {
    // get callback
    std::function<void(const std::string &)> log_message_;
    if (log_message != val::null())
    {
      log_message_ = get_callback(val(log_message));
    }

    auto ch = RoutingKit::ContractionHierarchy::build(
        node_count, tail, head, weight, log_message_, max_pop_count);

    return CH(RoutingKit::ContractionHierarchy(ch));
  }

  CH CH::build_given_rank(
      std::vector<unsigned> &rank,
      std::vector<unsigned> tail, std::vector<unsigned> head, std::vector<unsigned> weight,
      val log_message,
      unsigned max_pop_count)
  {
    // get callback
    std::function<void(const std::string &)> log_message_;
    if (log_message != val::null())
    {
      log_message_ = get_callback(val(log_message));
    }

    auto ch = RoutingKit::ContractionHierarchy::build_given_rank(
        rank, tail, head, weight, log_message_, max_pop_count);

    return CH(RoutingKit::ContractionHierarchy(ch));
  }

  CH CH::build_given_order(
      std::vector<unsigned> &order,
      std::vector<unsigned> tail, std::vector<unsigned> head, std::vector<unsigned> weight,
      val log_message,
      unsigned max_pop_count)
  {
    // get callback
    std::function<void(const std::string &)> log_message_;
    if (log_message != val::null())
    {
      log_message_ = get_callback(val(log_message));
    }

    auto ch = RoutingKit::ContractionHierarchy::build_given_order(
        order, tail, head, weight, log_message_, max_pop_count);

    return CH(RoutingKit::ContractionHierarchy(ch));
  }

  void CH::write(val write_js) const
  {
    auto fn = [write_js](const char *buffer, unsigned long long size)
    {
      auto b = val(typed_memory_view(size, buffer));
      write_js.call<void>("call", 0, b);
    };

    _ch.write(fn);
  }

  CH CH::read(val read_js)
  {
    auto fn = [read_js](char *buffer, unsigned long long size)
    {
      auto b = val(typed_memory_view(size, buffer));
      read_js.call<void>("call", 0, b);
    };

    auto ch = RoutingKit::ContractionHierarchy::read(fn);
    return CH(RoutingKit::ContractionHierarchy(ch));
  }

  CHQuery::CHQuery(const CH &ch) : _ch(const_cast<CH *>(&ch))
  {
    _query = new RoutingKit::ContractionHierarchyQuery(
        _ch->get());
  }

  CHQuery &CHQuery::reset()
  {
    _query->reset();
    return *this;
  }

  CHQuery &CHQuery::add_source(unsigned source, unsigned distance)
  {
    _query->add_source(source, distance);
    return *this;
  }

  CHQuery &CHQuery::add_target(unsigned target, unsigned distance)
  {
    _query->add_target(target, distance);
    return *this;
  }

  CHQuery &CHQuery::run()
  {
    _query->run();
    return *this;
  }

  unsigned CHQuery::get_used_source()
  {
    return _query->get_used_source();
  }

  unsigned CHQuery::get_used_target()
  {
    return _query->get_used_target();
  }

  unsigned CHQuery::get_distance()
  {
    return _query->get_distance();
  }

  std::vector<unsigned> CHQuery::get_node_path()
  {
    return _query->get_node_path();
  }

  std::vector<unsigned> CHQuery::get_arc_path()
  {
    return _query->get_arc_path();
  }

  CHQuery &CHQuery::reset_source()
  {
    _query->reset_source();
    return *this;
  }

  CHQuery &CHQuery::pin_targets(std::vector<unsigned> &targets)
  {
    _query->pin_targets(targets);
    return *this;
  }

  CHQuery &CHQuery::run_to_pinned_targets()
  {
    _query->run_to_pinned_targets();
    return *this;
  }

  std::vector<unsigned> CHQuery::get_distances_to_targets()
  {
    return _query->get_distances_to_targets();
  }

  CHQuery &CHQuery::reset_target()
  {
    _query->reset_target();
    return *this;
  }

  CHQuery &CHQuery::pin_sources(std::vector<unsigned> &sources)
  {
    _query->pin_sources(sources);
    return *this;
  }

  CHQuery &CHQuery::run_to_pinned_sources()
  {
    _query->run_to_pinned_sources();
    return *this;
  }

  std::vector<unsigned> CHQuery::get_distances_to_sources()
  {
    return _query->get_distances_to_sources();
  }

  std::vector<unsigned> CHQuery::get_used_sources_to_targets()
  {
    return _query->get_used_sources_to_targets();
  }

  std::vector<unsigned> CHQuery::get_used_targets_to_sources()
  {
    return _query->get_used_targets_to_sources();
  }

} // namespace RoutingKitJS

EMSCRIPTEN_BINDINGS(RoutingKitJs_CH)
{
  register_vector<unsigned>("VectorUint");
  register_vector<float>("VectorFloat");

  class_<RoutingKitJS::CH>("CH")
      .class_function("build", &RoutingKitJS::CH::build)
      .class_function("build_given_rank", &RoutingKitJS::CH::build_given_rank)
      .class_function("build_given_order", &RoutingKitJS::CH::build_given_order)
      .class_function("read", &RoutingKitJS::CH::read)
      .function("node_count", &RoutingKitJS::CH::node_count)
      .function("get_order", &RoutingKitJS::CH::get_order)
      .function("get_rank", &RoutingKitJS::CH::get_rank)
      .function("write", &RoutingKitJS::CH::write);

  class_<RoutingKitJS::CHQuery>("CHQuery")
      .constructor<const RoutingKitJS::CH &>()
      .function("reset", &RoutingKitJS::CHQuery::reset)
      .function("add_source", &RoutingKitJS::CHQuery::add_source)
      .function("add_target", &RoutingKitJS::CHQuery::add_target)
      .function("run", &RoutingKitJS::CHQuery::run)
      .function("get_used_source", &RoutingKitJS::CHQuery::get_used_source)
      .function("get_used_target", &RoutingKitJS::CHQuery::get_used_target)
      .function("get_distance", &RoutingKitJS::CHQuery::get_distance)
      .function("get_node_path", &RoutingKitJS::CHQuery::get_node_path)
      .function("get_arc_path", &RoutingKitJS::CHQuery::get_arc_path)
      .function("reset_source", &RoutingKitJS::CHQuery::reset_source)
      .function("pin_targets", &RoutingKitJS::CHQuery::pin_targets)
      .function("run_to_pinned_targets", &RoutingKitJS::CHQuery::run_to_pinned_targets)
      .function("get_distances_to_targets", &RoutingKitJS::CHQuery::get_distances_to_targets)
      .function("reset_target", &RoutingKitJS::CHQuery::reset_target)
      .function("pin_sources", &RoutingKitJS::CHQuery::pin_sources)
      .function("run_to_pinned_sources", &RoutingKitJS::CHQuery::run_to_pinned_sources)
      .function("get_distances_to_sources", &RoutingKitJS::CHQuery::get_distances_to_sources)
      .function("get_used_sources_to_targets", &RoutingKitJS::CHQuery::get_used_sources_to_targets)
      .function("get_used_targets_to_sources", &RoutingKitJS::CHQuery::get_used_targets_to_sources);
}