/*
Title : Actions
Author: Edward R. Gonzalez

Description: 
This was a little experiment of mine to mess with action binding...

Allows for non-member functions to be binded to an action, implements a functioning queue as well.

TODO: Possibly add support for member functions. Have it so that deduction of delegate typef is not required to add to queue properly (right now it does, see input procedure for example);
*/


#pragma once




#include "Cpp_Alias.hpp"



namespace Actions
{
	struct IAction
	{
		virtual void DoAction() = NULL;
	};

	template<typename FunctionType, typename... ActionParams>
	struct AAction : IAction
	{
	public:
		using ActionType = function< FunctionType >;

		AAction(const ActionType& _actionToAssign, const ActionParams&... _params) :
			action(_actionToAssign),
			params(_params...     ),
			done  (false          )
		{};

		bool Used()
		{
			return done;
		}

		bool IsSame(const ActionType& _action, const ActionParams&... _paramsForAction)
		{
			tuple<ActionParams...> paramsToCheck(_paramsForAction...);

			if (params == paramsToCheck && SameAction(_action))
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		bool SameAction(const ActionType& _action)
		{
			if (action.target<FunctionType*>() == _action.target<FunctionType*>())
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		void ReInitalize(const ActionParams&... _params)
		{
			params = tuple<ActionParams...>(_params...);

			done = false;
		}

	protected:
		virtual void DoAction_Implementation(const ActionParams&... _params) { action(_params...); }

		template<size_t... TuplePackIndex>                                                           // TuplePackSequence<TuplePackIndex...>
		void ExpandTuple_CallDoActionImplementaiton(const tuple<ActionParams...>& _paramsToExpand, std::index_sequence   <TuplePackIndex...>)
		{
			                              // ExpandTuplePack<TuplePackIndex>
			DoAction_Implementation(std::get<TuplePackIndex>(_paramsToExpand)...);
		}

		tuple<ActionParams...> params;

		const ActionType& action;

		bool done;

	public:   // IAction

		virtual void DoAction() override
		{
			ExpandTuple_CallDoActionImplementaiton
			(
				params,
				// MakeTuplePackSequence  <ActionParams...>()
				std::index_sequence_for<ActionParams...>()
			);

			done = true;
		};
	};

	// TODO: This doesn't work yet...
	template<typename ObjectType, typename FunctionType, typename... ActionParams>
	class AAction_ObjectBound : public AAction<FunctionType, ActionParams...>
	{
	public:
		using Base = AAction<FunctionType, ActionParams...>;

		using ActionType = function<FunctionType>;

		AAction_ObjectBound(ObjectType& _objectRef, const ActionType& _actionToAssign, const ActionParams&... _params) :
			AAction<FunctionType, ActionParams...>::action(_actionToAssign),
			AAction<FunctionType, ActionParams...>::params(_params...     ),
			object                                        (_objectRef     )
		{}


		bool IsSame(const ObjectType& _object, const ActionType& _action, const ActionParams&... _params)
		{
			if (SameObject(_object) && SameAction(_action))
			{
				tuple<ActionParams...> paramsToCheck(_params...);

				if (AAction<FunctionType, ActionParams...>::params == paramsToCheck)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}

		bool SameObject(const ObjectType& _object)
		{
			if (Address(object) == Address(_object))
			{
				return true;
			}
			else
			{
				return false;
			}
		}


	protected:
		virtual void DoAction_Implementation(const ActionParams&... _params) override
		{ 
			(&object).*AAction<FunctionType, ActionParams...>::action(_params...);
		}

		template<size_t... TuplePackIndex>                                                           // TuplePackSequence<TuplePackIndex...>
		void ExpandTuple_CallDoActionImplementaiton(const tuple<ActionParams...>& _paramsToExpand, std::index_sequence   <TuplePackIndex...>)
		{
			// ExpandTuplePack<TuplePackIndex>
			DoAction_Implementation(std::get<TuplePackIndex>(_paramsToExpand)...);
		}


		ObjectType& object;


	public:   // IAction

		virtual void DoAction() override
		{
			ExpandTuple_CallDoActionImplementaiton
			(
				AAction<FunctionType, ActionParams...>::params,
				// MakeTuplePackSequence  <ActionParams...>()
				std::index_sequence_for<ActionParams...>()
			);

			AAction<FunctionType, ActionParams...>::params::done = true;
		};
	};

	struct ActionPool_Dynamic
	{
		template<typename Type>
		using AllocationsOf = std::forward_list<Type>;
		
		using Managed_AAction   = shared_ptr     < IAction                          >;
		using Managed_AActions  = AllocationsOf  < Managed_AAction                  >;
		using AActions_Registry = std::map       <type_index      , Managed_AActions>;

	public:

		template<typename Entry>
		bool Contains(Entry& _entry)
		{
			return _entry != aActions_Available.end() ? true : false;
		}

		Managed_AActions& Make_Managed_Actions()
		{
			mAAaction_Allocations.push_front(make_shared<Managed_AActions>());

			return *(mAAaction_Allocations.front().get());
		}

		template<typename FunctionType, typename... ActionParams>
		IAction* Request_AAction(const function< FunctionType>& _actionToQueue, const ActionParams&... _paramsForAction)
		{
			using ActionType = AAction < FunctionType, ActionParams...>;

			type_index AActionID = typeid(ActionType);

			auto possibleEntry = aActions_Available.find(AActionID);

			if (Contains(possibleEntry))
			{
				using Element = decltype(possibleEntry->second.begin());

				for (Element possibleAction = possibleEntry->second.begin(); possibleAction != possibleEntry->second.end(); possibleAction++)
				{
					ActionType* castedEntry = static_cast<ActionType*>(possibleAction->get());

					if (castedEntry->IsSame(_actionToQueue, _paramsForAction...))
					{
						return castedEntry;
					}
					else if (castedEntry->Used() && castedEntry->SameAction(_actionToQueue))
					{
						castedEntry->ReInitalize(_paramsForAction...);

						return castedEntry;
					}
				}

				shared_ptr< IAction> newAction = make_shared< AAction<FunctionType, ActionParams...>>(_actionToQueue, _paramsForAction...);
				            IAction* returnRef = newAction.get                                                                          ();

				aActions_Available.at(AActionID).push_front(newAction);

				return returnRef;
			}

			shared_ptr< IAction> newAction = make_shared< AAction<FunctionType, ActionParams...>>(_actionToQueue, _paramsForAction...);
			            IAction* returnRef = newAction.get                                                                          ();

			aActions_Available.insert(make_pair(AActionID, Make_Managed_Actions()));

			aActions_Available.at(AActionID).push_front(newAction);

			return returnRef;
		}

		template<typename ObjectType, typename FunctionType, typename... ActionParams>
		IAction* Request_AAction(const ObjectType& _objectRef, const function< FunctionType>& _actionToQueue, const ActionParams&... _paramsForAction)
		{
			using ActionType = AAction_ObjectBound<ObjectType, FunctionType, ActionParams...>;

			type_index AActionID = typeid(ActionType);

			auto possibleEntry = aActions_Available.find(AActionID);

			if (Contains(possibleEntry))
			{
				using Element = decltype(possibleEntry->second.begin());

				for (Element possibleAction = possibleEntry->second.begin(); possibleAction != possibleEntry->second.end(); possibleAction++)
				{
					ActionType* castedEntry = static_cast<ActionType*>(possibleAction->get());

					if (castedEntry->IsSame(_actionToQueue, _paramsForAction...))
					{
						return castedEntry;
					}
					else if (castedEntry->Used() && castedEntry->SameAction(_actionToQueue))
					{
						castedEntry->ReInitalize(_paramsForAction...);

						return castedEntry;
					}
				}

				shared_ptr< IAction> newAction = make_shared< AAction<FunctionType, ActionParams...>>(_actionToQueue, _paramsForAction...);
				            IAction* returnRef = newAction.get                                                                          ();

				aActions_Available.at(AActionID).push_front(newAction);

				return returnRef;
			}

			shared_ptr< IAction> newAction = make_shared< AAction<FunctionType, ActionParams...>>(_actionToQueue, _paramsForAction...);
			            IAction* returnRef = newAction.get                                                                          ();

			aActions_Available.insert(std::make_pair(AActionID, Make_Managed_Actions()));

			aActions_Available.at(AActionID).push_front(newAction);

			return returnRef;
		}


	private:
		AllocationsOf< shared_ptr<Managed_AActions> > mAAaction_Allocations;

		AActions_Registry aActions_Available;
	};

	ActionPool_Dynamic DefaultActionPool_Dynamic;



	struct ActionQueue
	{
		using QueueType = std::deque< IAction*>;

		
	public:
		template<typename FunctionType, typename... ActionParams>
		void AddToQueue(const function< FunctionType>& _actionToQueue, const ActionParams&... _paramsForAction)
		{
			using GeneratedActionType = AAction<FunctionType, ActionParams...>;

			IAction* actionRequested = DefaultActionPool_Dynamic.Request_AAction(_actionToQueue, _paramsForAction...);

			if (HasAction())
			{
				bool found = false;

				using Element = decltype(actionQueue.begin());

				for (Element element = actionQueue.begin(); element != actionQueue.end(); element++)
				{
					if ( (*element) == actionRequested )
					{
						found = true;
					}
				}

				if (not found)
				{
					actionQueue.push_front(actionRequested);
				}
			}
			else
			{
				actionQueue.push_front(actionRequested);
			}
		}

		template<typename ObjectType, typename FunctionType, typename... ActionParams>
		void AddToQueue(const ObjectType& _objectRef, const function< FunctionType>& _actionToQueue, const ActionParams&... _paramsForAction)
		{
			using GeneratedActionType = AAction_ObjectBound<ObjectType, FunctionType, ActionParams...>;

			IAction* actionRequested = DefaultActionPool_Dynamic.Request_AAction(_objectRef, _actionToQueue, _paramsForAction...);

			if (HasAction())
			{
				bool found = false;

				using Element = decltype(actionQueue.begin());

				for (Element element = actionQueue.begin(); element != actionQueue.end(); element++)
				{
					if ((*element) == actionRequested)
					{
						found = true;
					}
				}

				if (not found)
				{
					actionQueue.push_front(actionRequested);
				}
			}
			else
			{
				actionQueue.push_front(actionRequested);
			}
		}

		void DoNextAction()
		{
			if (actionQueue.size() > 0)
			{
				actionQueue.back()->DoAction();

				actionQueue.pop_back();
			}
		}

		bool HasAction()
		{
			return actionQueue.size() > 0;
		}


	private:

		QueueType actionQueue;
	};
}
