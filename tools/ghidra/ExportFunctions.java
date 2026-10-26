// Export every function's entry, body size, name and name source as CSV.
// usage: analyzeHeadless ... -postScript ExportFunctions.java <out.csv>
// @category RallyTrophy

import java.io.PrintWriter;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.AddressSetView;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;

public class ExportFunctions extends GhidraScript {
	@Override
	protected void run() throws Exception {
		String out = getScriptArgs()[0];
		try (PrintWriter w = new PrintWriter(out)) {
			w.println("entry,size,align16,thunk,source,calling_convention,name");
			FunctionIterator it = currentProgram.getFunctionManager().getFunctions(true);
			while (it.hasNext()) {
				Function f = it.next();
				AddressSetView body = f.getBody();
				long entry = f.getEntryPoint().getOffset();
				long size = body.getMaxAddress().getOffset() - body.getMinAddress().getOffset() + 1;
				w.printf("0x%x,%d,%d,%d,%s,%s,\"%s\"%n", entry, size, entry % 16 == 0 ? 1 : 0,
					f.isThunk() ? 1 : 0, f.getSymbol().getSource(), f.getCallingConventionName(),
					f.getName(true).replace("\"", "\"\""));
			}
		}
		println("exported functions to " + out);
	}
}
