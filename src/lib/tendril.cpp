#include <ecto/tendril.hpp>
#include <ecto/common_tags.hpp>
namespace ecto
{

  boost::shared_ptr<tendril::holder_base> tendril::none_holder_(new holder<none>(none()));
  namespace
  {
    bool isBoostPython(const tendril& t)
    {
      return t.is_type<boost::python::object>();
    }
  }

  tendril::tendril() :
      holder_(none_holder_)
    , dirty_(false)
    , default_(false)
    , user_supplied_(false)
  {
    //impl_ is never not initialized
  }

  tendril::~tendril()
  {}

  tendril::tendril(const tendril& rhs) :
      holder_(rhs.holder_->clone())
    , dirty_(false)
    , default_(rhs.default_)
    , user_supplied_(rhs.user_supplied_)
    , constraints_(rhs.constraints_)
  {}


  tendril::tendril(holder_base::ptr impl) :
      holder_(impl)
    , dirty_(false)
    , default_(false)
    , user_supplied_(false)
  {}


  tendril& tendril::operator=(const tendril& rhs)
  {
    if (this == &rhs)
      return *this;
    holder_ = rhs.holder_->clone();
    dirty_ = rhs.dirty_;
    default_ = rhs.default_;
    constraints_ = rhs.constraints_;
    return *this;
  }

  void tendril::copy_value(const tendril& rhs)
  {
    if (this == &rhs)
      return;
    if (is_type<none>())
    {
      holder_ = rhs.holder_->clone();
    }
    else
    {
      enforce_compatible_type(rhs);
      *holder_ = *rhs.holder_;
    }
    mark_dirty();
  }


  void tendril::set_doc(const std::string& doc_str)
  {
    constrain(constraints::Doc(doc_str));
  }

  tendril::holder_base& 
  tendril::holder_base::operator=(const tendril::holder_base& rhs)
  {
    if (this == &rhs)
      return *this;
    rhs.copy_to(*this);
    return *this;
  }

  tendril::holder_base::~holder_base()
  {
  }

  boost::python::object
  tendril::extract() const
  {
    return holder_->getPython();
  }

  void tendril::enqueue(FnT fn)
  {
    boost::mutex::scoped_lock lock(mtx_);
    queue_.push_front(fn);
  }

  void tendril::exec_queue()
  {
    boost::mutex::scoped_lock lock(mtx_);
    while(!queue_.empty())
    {
      queue_.back()();
      queue_.pop_back();
    }
  }

  void
  tendril::set(boost::python::object o)
  {
    if (is_type<none>())
    {
      holder_.reset(new holder<boost::python::object>(o));
    }
    else
    {
      holder_->setPython(o);
    }
    mark_dirty();
  }
  void tendril::notify()
  {
    exec_queue();
    if (dirty())
    {
      holder_->trigger_callback();
    }
    mark_clean();
  }
  tendril& tendril::constrain(constraints::ptr c)
  {
    constraints_[c->key()] = c;
    return *this;
  }
  constraints::ptr tendril::get_constraint(const std::string& key) const
  {
    if(constraints_.count(key)) return constraints_.find(key)->second;
    return constraints::ptr();
  }

  std::string
  tendril::doc() const
  {
    return constrained<std::string>(constraints::Doc("TODO: Doc me."));
  }

  void
  tendril::required(bool b)
  {
    constrain(constraints::Required(b));
  }

  bool
  tendril::required() const
  {
    return constrained<bool>(constraints::Required(false));;
  }

  bool
  tendril::user_supplied() const
  {
    return user_supplied_;
  }

  bool
  tendril::has_default() const
  {
    return default_;
  }

  bool
  tendril::dirty() const
  {
    return dirty_;
  }

  //! The tendril has notified its callback if one was registered since it was changed.
  bool
  tendril::clean() const
  {
    return !dirty_;
  }

  tendril&
  tendril::constrain(const constraints::constraint_base& c)
  {
    return constrain(c.clone());
  }

  bool
  tendril::same_type(const tendril& rhs) const
  {
    return type_name() == rhs.type_name();
  }

  bool
  tendril::compatible_type(const tendril& rhs) const
  {
    if (same_type(rhs))
      return true;
    return is_type<none>() || rhs.is_type<none>() || is_type<boost::python::object>()
           || rhs.is_type<boost::python::object>();
  }

  void
  tendril::enforce_compatible_type(const tendril& rhs) const
  {
    if (!compatible_type(rhs))
    {
      throw except::TypeMismatch(type_name() + " is not a " + rhs.type_name());
    }
  }


  void
  tendril::mark_dirty()
  {
    dirty_ = true;
    user_supplied_ = true;
  }
  void
  tendril::mark_clean()
  {
    dirty_ = false;
  }

}

//ecto::tendril&
//operator<<(ecto::tendril& rhs,const ecto::constraints::constraint_base& constraint)
//{
//  rhs.add_constraint(constraint.clone());
//  return rhs;
//}
