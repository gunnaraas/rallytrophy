// Write Ghidra pseudocode for every function in [lo, hi) to <outdir>/0x<entry>.c,
// so drafting agents can work without an interactive REA session.
// usage: analyzeHeadless ... -postScript DecompileRange.java <lo> <hi> <outdir>
// @category RallyTrophy

import java.io.File;
import java.io.PrintWriter;

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;

public class DecompileRange extends GhidraScript {
	@Override
	protected void run() throws Exception {
		String[] args = getScriptArgs();
		long lo = Long.decode(args[0]), hi = Long.decode(args[1]);
		File outdir = new File(args[2]);
		outdir.mkdirs();
		DecompInterface ifc = new DecompInterface();
		ifc.openProgram(currentProgram);
		for (Function f : currentProgram.getFunctionManager().getFunctions(toAddr(lo), true)) {
			long entry = f.getEntryPoint().getOffset();
			if (entry >= hi)
				break;
			DecompileResults r = ifc.decompileFunction(f, 120, monitor);
			try (PrintWriter w = new PrintWriter(new File(outdir, String.format("0x%x.c", entry)))) {
				w.println(r.decompileCompleted() ? r.getDecompiledFunction().getC() : "// decompile failed: " + r.getErrorMessage());
			}
		}
		ifc.dispose();
	}
}
