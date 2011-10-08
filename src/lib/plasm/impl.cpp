#include <ecto/all.hpp>

#include "plasm/impl.hpp"

namespace ecto {

  using namespace except;

  using graph::graph_t;
  using graph::edge;

  namespace
  {
    using boost::tie;
    using boost::add_vertex;

    graph::edge_ptr
    make_edge(const std::string& fromport, const std::string& toport)
    {
      graph::edge_ptr eptr(new graph::edge(fromport, toport));
      return eptr;
    }
  } // namespace


  plasm::impl::impl() {}

    //insert a cell into the graph, will retrieve the
    //vertex descriptor if its already in the graph...
  graph_t::vertex_descriptor
  plasm::impl::insert_module(cell_ptr m)
  {
    //use the vertex map to look up the graphviz descriptor (reverse lookup)
    ModuleVertexMap::iterator it = mv_map.find(m);
    if (it != mv_map.end())
      return it->second;
    graph_t::vertex_descriptor d = add_vertex(m, graph);
    mv_map.insert(std::make_pair(m, d));
    return d;
  }

  void
  plasm::impl::connect(cell_ptr from, std::string output, cell_ptr to, std::string input)
  {
    //connect does all sorts of type checking so that connections are always valid.
    tendril_ptr from_port, to_port;
    try {
      from_port = from->outputs[output];
    }
    catch (ecto::NonExistant& e) {
      e << cell_name(from->name())
        << tendril_key(output)
        << which_tendrils("outputs")
        ;
      throw;
    }

    try {
      to_port = to->inputs[input];
    }
    catch (ecto::NonExistant& e) {
      e << cell_name(from->name())
        << tendril_key(input)
        << which_tendrils("inputs")
        ;
      throw;
    }
    //throw if the types are bad... Don't allow erroneous graph construction
    //also this is more local to the error.
    if (!to_port->compatible_type(*from_port))
      {
        std::string s;
        BOOST_THROW_EXCEPTION(TypeMismatch()
                               << from_typename(from_port->type_name())
                               << from_key(output)
                               << from_cell(from->name())
                               << to_typename(to_port->type_name())
                               << to_key(input)
                               << to_cell(to->name()));
      }

    graph_t::vertex_descriptor fromv = insert_module(from), tov = insert_module(to);
    graph::edge_ptr new_edge = make_edge(output, input);

    //assert that the new edge does not violate inputs that are already connected.
    //RULE an input may only have one source.
    graph_t::in_edge_iterator inbegin, inend;
    tie(inbegin, inend) = boost::in_edges(tov, graph);
    while (inbegin != inend)
      {
        graph::edge_ptr e = graph[*inbegin];
        if (e->to_port() == new_edge->to_port())
          {
            BOOST_THROW_EXCEPTION(AlreadyConnected()
                                  << cell_name(to->name())
                                  << tendril_key(e->to_port())
                                  );
          }
        ++inbegin;
      }

    bool added;
    graph_t::edge_descriptor ed;
    tie(ed, added) = boost::add_edge(fromv, tov, new_edge, graph);
    if (!added)
      {
        BOOST_THROW_EXCEPTION(EctoException()
                              << diag_msg("plasm failed to connect cell input/outputs")
                              << from_cell(from->name())
                              << from_key(output)
                              << to_cell(to->name())
                              << to_key(input));
      }
  }

  void
  plasm::impl::disconnect(cell_ptr from, std::string output, cell_ptr to, std::string input)
  {
    graph_t::vertex_descriptor fromv = insert_module(from), tov = insert_module(to);
    boost::remove_edge(fromv, tov, graph);
  }

}
