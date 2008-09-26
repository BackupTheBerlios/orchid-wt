function Recursion(deep) {
	this.query = function(request) {
		write(request, "recursion-deep: "+deep);
	}
	this.childList = function() {
		var list = new Array();
		for(var i = 1; i <= deep; ++i)
			list.push("child"+i);
		return list;
	}
	this.getChild = function(name) {
		return new Recursion(deep+1);
	}
	return this;
}

function MyResource() {
	this.query = function(request) {
		write(request, "test");
	};
	this.childList = function() {
		var list = new Array();
		list.push("child");
		return list;
	};
	this.getChild = function(name) {
		return new Recursion(1);
	};
	return this;
}