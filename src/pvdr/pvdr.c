/*
 * Copyright 2015-2016 Tobias Waldekranz <tobias@waldekranz.com>
 *
 * This file is part of ply.
 *
 * ply is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, under the terms of version 2 of the
 * License.
 *
 * ply is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
 * License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ply.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <errno.h>
#include <string.h>

#include "../ply.h"
#include "pvdr.h"

TAILQ_HEAD(pvdrs, pvdr);
static struct pvdrs pvdr_list = TAILQ_HEAD_INITIALIZER(pvdr_list);


pvdr_t *pvdr_find(const char *pspec)
{
	pvdr_t *pvdr;
	char *colon;

	colon = strchr(pspec, ':');
	if (!colon)
		return NULL;
	
	TAILQ_FOREACH(pvdr, &pvdr_list, node) {
		if (!strncmp(pvdr->name, pspec, colon - pspec))
			return pvdr;
	}

	return NULL;
}

int pvdr_resolve(node_t *script)
{	
	node_t *probe;
	pvdr_t *pvdr;

	for (probe = script->script.probes; probe; probe = probe->next) {
		pvdr = pvdr_find(probe->string);
		if (!pvdr) {
			_e("no provider matching '%s'", probe->string);
			return -ENOENT;
		}

		probe->dyn.probe.pvdr = pvdr;
	}

	return 0;
}

void pvdr_register(pvdr_t *pvdr)
{
	TAILQ_INSERT_TAIL(&pvdr_list, pvdr, node);
}
