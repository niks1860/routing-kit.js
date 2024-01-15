#include <routingkit/customizable_contraction_hierarchy.h>
#include <routingkit/nested_dissection.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>

#include <routingkitjs/cch.h>

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

  CCH::CCH(
      unsigned node_count,
      const std::vector<unsigned> &tail,
      const std::vector<unsigned> &head,
      const std::vector<float> &lat,
      const std::vector<float> &lon,
      val log_message)
  {
    // get callback
    std::function<void(const std::string &)> log_message_;
    if (log_message != val::null())
    {
      log_message_ = get_callback(val(log_message));
    }

    auto order = RoutingKit::compute_nested_node_dissection_order_using_inertial_flow(
        node_count, tail, head, lat, lon, log_message_);

    _cch = new RoutingKit::CustomizableContractionHierarchy(
        order, tail, head, log_message_);
  }

  CCHMetric::CCHMetric(
      const CCH &cch,
      const std::vector<unsigned> &weight)
      : _cch(const_cast<CCH *>(&cch)), _weights(const_cast<std::vector<unsigned> *>(&weight))
  {
    _metric = new RoutingKit::CustomizableContractionHierarchyMetric(
        _cch->get(), weight);
  }

  CCHMetric &CCHMetric::reset(const std::vector<unsigned> &weight)
  {
    _metric->reset(weight);
    return *this;
  }

  CCHMetric &CCHMetric::customize()
  {
    _metric->customize();
    return *this;
  }

  CCHQuery::CCHQuery(
      const CCHMetric &metric) : _metric(const_cast<CCHMetric *>(&metric))
  {
    _query = new RoutingKit::CustomizableContractionHierarchyQuery(
        _metric->get());
  }

  CCHQuery &CCHQuery::reset()
  {
    _query->reset();
    return *this;
  }

  CCHQuery &CCHQuery::add_source(unsigned source, unsigned distance)
  {
    _query->add_source(source, distance);
    return *this;
  }

  CCHQuery &CCHQuery::add_target(unsigned target, unsigned distance)
  {
    _query->add_target(target, distance);
    return *this;
  }

  CCHQuery &CCHQuery::run()
  {
    _query->run();
    return *this;
  }

  unsigned CCHQuery::get_used_source()
  {
    return _query->get_used_source();
  }

  unsigned CCHQuery::get_used_target()
  {
    return _query->get_used_target();
  }

  unsigned CCHQuery::get_distance()
  {
    return _query->get_distance();
  }

  std::vector<unsigned> CCHQuery::get_node_path()
  {
    return _query->get_node_path();
  }

  std::vector<unsigned> CCHQuery::get_arc_path()
  {
    return _query->get_arc_path();
  }

  CCHQuery &CCHQuery::reset_source()
  {
    _query->reset_source();
    return *this;
  }

  CCHQuery &CCHQuery::pin_targets(std::vector<unsigned> &targets)
  {
    _query->pin_targets(targets);
    return *this;
  }

  CCHQuery &CCHQuery::run_to_pinned_targets()
  {
    _query->run_to_pinned_targets();
    return *this;
  }

  std::vector<unsigned> CCHQuery::get_distances_to_targets()
  {
    return _query->get_distances_to_targets();
  }

  CCHQuery &CCHQuery::reset_target()
  {
    _query->reset_target();
    return *this;
  }

  CCHQuery &CCHQuery::pin_sources(std::vector<unsigned> &sources)
  {
    _query->pin_sources(sources);
    return *this;
  }

  CCHQuery &CCHQuery::run_to_pinned_sources()
  {
    _query->run_to_pinned_sources();
    return *this;
  }

  std::vector<unsigned> CCHQuery::get_distances_to_sources()
  {
    return _query->get_distances_to_sources();
  }

} // namespace RoutingKitJS

EMSCRIPTEN_BINDINGS(RoutingKitJs_CCH)
{
  register_vector<unsigned>("VectorUint");
  register_vector<float>("VectorFloat");

  class_<RoutingKitJS::CCH>("CCH")
      .constructor<unsigned, const std::vector<unsigned> &, const std::vector<unsigned> &, const std::vector<float> &, const std::vector<float> &, val>()
      .function("node_count", &RoutingKitJS::CCH::node_count)
      .function("input_arc_count", &RoutingKitJS::CCH::input_arc_count)
      .function("cch_arc_count", &RoutingKitJS::CCH::cch_arc_count);

  class_<RoutingKitJS::CCHMetric>("CCHMetric")
      .constructor<const RoutingKitJS::CCH &, const std::vector<unsigned> &>()
      .function("reset", &RoutingKitJS::CCHMetric::reset)
      .function("customize", &RoutingKitJS::CCHMetric::customize);

  class_<RoutingKitJS::CCHQuery>("CCHQuery")
      .constructor<const RoutingKitJS::CCHMetric &>()
      .function("reset", &RoutingKitJS::CCHQuery::reset)
      .function("add_source", &RoutingKitJS::CCHQuery::add_source)
      .function("add_target", &RoutingKitJS::CCHQuery::add_target)
      .function("run", &RoutingKitJS::CCHQuery::run)
      .function("get_used_source", &RoutingKitJS::CCHQuery::get_used_source)
      .function("get_used_target", &RoutingKitJS::CCHQuery::get_used_target)
      .function("get_distance", &RoutingKitJS::CCHQuery::get_distance)
      .function("get_node_path", &RoutingKitJS::CCHQuery::get_node_path)
      .function("get_arc_path", &RoutingKitJS::CCHQuery::get_arc_path)
      .function("reset_source", &RoutingKitJS::CCHQuery::reset_source)
      .function("pin_targets", &RoutingKitJS::CCHQuery::pin_targets)
      .function("run_to_pinned_targets", &RoutingKitJS::CCHQuery::run_to_pinned_targets)
      .function("get_distances_to_targets", &RoutingKitJS::CCHQuery::get_distances_to_targets)
      .function("reset_target", &RoutingKitJS::CCHQuery::reset_target)
      .function("pin_sources", &RoutingKitJS::CCHQuery::pin_sources)
      .function("run_to_pinned_sources", &RoutingKitJS::CCHQuery::run_to_pinned_sources)
      .function("get_distances_to_sources", &RoutingKitJS::CCHQuery::get_distances_to_sources);
}
